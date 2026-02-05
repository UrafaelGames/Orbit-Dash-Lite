#include "PlayScene.h"
#include "RingNode.h"
#include "PlayerObject.h"
#include "ODCenterBox.h"
#include "OrbitingObstacle.h"
#include "OBB2D.h"
#include "CCGameManager.h"
#include "EndGameLayer.h"
#include "CCMenuItemSpriteExtra.h"
#include "GamePauseLayer.h"
#include "CCAudioManager.h"
#include <cstdlib>
#include <ctime>

USING_NS_CC;

CCScene* PlayScene::createScene()
{
    cocos2d::CCScene* scene = CCScene::create();
    PlayScene* layer = new PlayScene();
    if (layer && layer->init()) {
        layer->autorelease();
        scene->addChild(layer);
    }
    else {
        CC_SAFE_DELETE(layer);
    }
    return scene;
}

bool PlayScene::init()
{
    if (!CCLayer::init()) return false;

    srand(time(NULL)); 

    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCPoint center = ccp(origin.x + vs.width / 2, origin.y + vs.height / 2);

    CCGameManager::sharedManager()->resetCurrentScore();
    CCGameManager::sharedManager()->incrementGamesPlayed();

    CCAudioManager::sharedManager()->stopBgMusic();
    CCAudioManager::sharedManager()->playBgMusic("Digital Zenith.mp3", true);

    m_player = NULL;
    m_ring = NULL;
    m_centerBox = NULL;
    m_score = 0;
    m_playerHitBoxThisFrame = false;
    m_nextObstacleScore = 600;
    m_obstacleInterval = 600;
    m_hitCooldown = 0.0f;

    m_scoreLabel = CCLabelBMFont::create("Score: 0", "fonts/BigFont-Gold.fnt");
    m_scoreLabel->setPosition(ccp(origin.x + vs.width / 2, origin.y + vs.height / 2 * 1.8f));
    addChild(m_scoreLabel, 100);

    CCSprite* pause = CCSprite::create("OD_PauseButton.png");
    CCMenuItemSpriteExtra* pauseBtn = CCMenuItemSpriteExtra::create(
        pause,
        this,
        menu_selector(PlayScene::pauseGame)
    );
    pauseBtn->setPosition(ccp(origin.x + vs.width / 2 * 0.83f, origin.y + vs.height / 2 * 0.92f));
    pauseBtn->setScale(1.0f);
    pauseBtn->setScaleMultiplier(1.2f);

    CCMenu* menu = CCMenu::create(pauseBtn, NULL);
    //menu->setIgnoreAnchorPointForPosition(false);
    this->addChild(menu, 2);

    CCSprite* bg = CCSprite::create("OD_GameBackground.png");
    bg->setContentSize(vs);
    bg->setPosition(ccp(origin.x + vs.width / 2, origin.y + vs.height / 2));
    this->addChild(bg, 0);

    RingNode* ring = RingNode::create("OD_Ring.png", 50.f);
    if (!ring) {
        CCLOG("Error creating ring!");
        return false;
    }
    ring->setPosition(center);
    this->addChild(ring);
    m_ring = ring;

    m_centerBox = ODCenterBox::create("OD_Box.png", 3);
    m_centerBox->setPosition(CCPointZero);
    ring->addChild(m_centerBox, 10);

    m_centerBox->setOnDestroyCallback(std::bind(&PlayScene::onBoxDestroyed, this, std::placeholders::_1));

    PlayerObject* player = PlayerObject::create("OD_Player.png");
    if (!player) {
        CCLOG("Error creating player!");
        return false;
    }
    player->attachToRing(ring);
    player->setAngularSpeed(180.f);
    addChild(player);
    m_player = player;

    CCTouchDispatcher* dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);

    float ringRadius = ring->getSprite()->getContentSize().width * 0.5f * ring->getSprite()->getScaleX();
    int randomChoice = rand() % 2;

    OrbitingObstacle* obs = NULL;
    if (randomChoice == 0) {
        obs = OrbitingObstacle::create(
            "OD_Obstacle1.png",
            ringRadius * 0.85f,
            60.f,
            0.f,
            0.f
        );
    }
    else {
        obs = OrbitingObstacle::create(
            "OD_Obstacle2.png",
            ringRadius * 0.8f,
            -60.f,
            180.f,
            90.f
        );
    }

    if (obs) {
        ring->addChild(obs);
        m_obstacles.push_back(obs);
    }

    this->scheduleUpdate();

    return true;
}

bool PlayScene::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (!m_player || m_player->isLinearMoving())
        return true;

    if (m_ring) {
        m_ring->setColorTint(ccc3(0, 255, 0)); 
        this->scheduleOnce(schedule_selector(PlayScene::restoreRingColor), 0.15f);
    }
    m_player->onRingTap(nullptr); 

    return true;
}

void PlayScene::restoreRingColor(float dt)
{
    if (m_ring) {
        m_ring->setColorTint(ccc3(255, 255, 255)); 
    }
}

void PlayScene::onBoxDestroyed(int points)
{
    CCLOG("Box destroyed... %d points", points);
    updateScore(points);

    this->runAction(CCSequence::create(
        CCDelayTime::create(1.0f),
        CCCallFunc::create(this, callfunc_selector(PlayScene::spawnNewBox)),
        NULL
    ));
}

void PlayScene::update(float dt)
{
    if (m_hitCooldown > 0.0f) {
        m_hitCooldown -= dt;
    }

    checkCollisions();
}

void PlayScene::checkCollisions()
{
    if (!m_player) return;

    OBB2D pOBB = m_player->getOBB();

    for (size_t i = 0; i < m_obstacles.size(); i++) {
        OBBBase* obstacle = dynamic_cast<OBBBase*>(m_obstacles[i]);
        if (obstacle) {
            OBB2D obsOBB = obstacle->getOBB();

            if (pOBB.intersectsOBB(obsOBB)) {
                CCLOG("COLLISION with obstacle!");
                CCAudioManager::sharedManager()->playSoundEffect("560576__theplax__explosion-3.wav");

                this->unscheduleUpdate();

                for (size_t j = 0; j < m_obstacles.size(); j++) {
                    m_obstacles[j]->pauseSchedulerAndActions();
                }
                if (m_ring) m_ring->pauseSchedulerAndActions();
                if (m_player) m_player->pauseSchedulerAndActions();

                if (m_ring) {
                    m_ring->playImpactAnimation();
                }

                if (m_player) {
                    m_player->playDeathAnimation();
                }

                this->runAction(CCSequence::create(
                    CCDelayTime::create(0.5f), 
                    CCCallFunc::create(this, callfunc_selector(PlayScene::showGameOverLayer)),
                    NULL
                ));

                return;
            }
        }
    }

    // Verificar colisión con la caja central
    if (m_centerBox && !m_centerBox->isDestroyed()) {
        OBB2D boxOBB = m_centerBox->getOBB();

        if (pOBB.intersectsOBB(boxOBB)) {
            CCLOG("BOX COLLISION");

            if (m_hitCooldown <= 0.0f) {
                m_centerBox->hit();
                m_hitCooldown = 0.5f;
            }
            else {
                CCLOG("Cooldown: %.2f", m_hitCooldown);
            }
        }
    }
}

void PlayScene::showGameOverLayer()
{
    int currentScore = CCGameManager::sharedManager()->getCurrentScore();
    int bestScore = CCGameManager::sharedManager()->getBestScore();

    CCAudioManager::sharedManager()->stopBgMusic();
    CCAudioManager::sharedManager()->playSoundEffect("levelEnding.ogg");

    CCGameManager::sharedManager()->saveData();

    EndGameLayer* endLayer = EndGameLayer::create(currentScore, bestScore);
     this->addChild(endLayer, 10000);

    CCLOG("Game Over! Score: %d, Best: %d", currentScore, bestScore);
}

void PlayScene::drawOBB(const OBB2D& obb, const ccColor4F& color)
{
}

void PlayScene::updateScore(int points)
{
    bool onDevSettings = false;

    if (onDevSettings) {
        int basePoints = points;
        int finalPoints = basePoints;

        if (m_score >= 6000) {
            finalPoints = 300;
        }
        else if (m_score >= 1800) {
            finalPoints = basePoints * 12;
        }
        else if (m_score >= 1000) {
            finalPoints = basePoints * 6;
        }

        CCGameManager::sharedManager()->addScore(finalPoints);
        m_score = CCGameManager::sharedManager()->getCurrentScore();
    }
    else {
        CCGameManager::sharedManager()->addScore(points);
        m_score = CCGameManager::sharedManager()->getCurrentScore();
    }

    std::string scoreText = "Score: " + std::to_string(m_score);
    m_scoreLabel->setString(scoreText.c_str());

    m_scoreLabel->stopAllActions();
    m_scoreLabel->runAction(CCSequence::create(
        CCScaleTo::create(0.1f, 1.3f),
        CCScaleTo::create(0.1f, 1.0f),
        NULL
    ));

    checkAndSpawnObstacles();
}

void PlayScene::checkAndSpawnObstacles()
{
    bool onDevSettings = false;

    if (onDevSettings) {
        if (m_score >= m_nextObstacleScore) {
            spawnRandomObstacle();
            if (m_score >= 2000) {
                m_nextObstacleScore += 300;
            }
            else {
                m_nextObstacleScore += 600;
            }
        }
    }
    else {
        if (m_score >= m_nextObstacleScore) {
            spawnRandomObstacle();
            m_nextObstacleScore += m_obstacleInterval;
            m_obstacleInterval = MAX(200, m_obstacleInterval - 50);
        }
    }
}

void PlayScene::spawnRandomObstacle()
{
    if (!m_ring) return;

    float ringRadius = m_ring->getSprite()->getContentSize().width * 0.5f * m_ring->getSprite()->getScaleX();
    int obstacleCount = m_obstacles.size();
    float baseSpeed = 40.f + (obstacleCount * 10.f);

    std::string spriteFile = (rand() % 2 == 0) ? "OD_Obstacle1.png" : "OD_Obstacle2.png";

    float radiusPercent;
    if (rand() % 10 < 7) { // 70%
        radiusPercent = 0.45f + (rand() % 16) * 0.01f; // 0.45-0.60
    }
    else { // 30%
        radiusPercent = 0.70f + (rand() % 11) * 0.01f; // 0.70-0.80
    }
    float radius = ringRadius * radiusPercent;

    float speed = (rand() % 2 == 0) ? baseSpeed : -baseSpeed;
    float startAngle = (rand() % 360) * 1.0f;
    float additionalRotation = (spriteFile == "OD_Obstacle2.png") ? 90.f : 0.f;

    OrbitingObstacle* newObs = OrbitingObstacle::create(
        spriteFile,
        radius,
        speed,
        startAngle,
        additionalRotation
    );

    if (newObs) {
        m_ring->addChild(newObs);
        m_obstacles.push_back(newObs);
    }
}

void PlayScene::spawnNewBox()
{
    m_centerBox = ODCenterBox::create("OD_Box.png", 3);
    m_centerBox->setPosition(CCPointZero);
    m_ring->addChild(m_centerBox, 10);

    m_centerBox->setOnDestroyCallback(std::bind(&PlayScene::onBoxDestroyed, this, std::placeholders::_1));
}

void PlayScene::pauseGame(CCObject* sender)
{
    this->unscheduleUpdate();

    for (size_t i = 0; i < m_obstacles.size(); i++) {
        m_obstacles[i]->pauseSchedulerAndActions();
    }

    if (m_ring) m_ring->pauseSchedulerAndActions();
    if (m_player) m_player->pauseSchedulerAndActions();

     GamePauseLayer* layer = GamePauseLayer::create();
     this->addChild(layer, 9999);

    CCLOG("Game Paused");
}