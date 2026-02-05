#include "EndGameLayer.h"
#include "CCGameManager.h"
#include "CCMenuItemSpriteExtra.h"
#include "MenuScene.h"
#include "PlayScene.h"
#include "CCAudioManager.h"
#include <cstdlib>
#include <sstream>

USING_NS_CC;

EndGameLayer* EndGameLayer::create(int score, int bestScore)
{
    EndGameLayer* ret = new EndGameLayer();
    if (ret && ret->init(score, bestScore))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

void EndGameLayer::onEnter()
{
    CCLayerColor::onEnter();
    this->registerWithTouchDispatcher();
}

bool EndGameLayer::init(int score, int bestScore)
{
    if (!CCLayerColor::init()) return false;

    m_score = score;
    m_bestScore = bestScore;
    m_bgPanel = NULL;
    m_panelContainer = NULL;
    m_overlay = NULL;

    initPanel();
    initContent();

    // CCAudioManager::sharedManager()->pauseBgMusic();

    return true;
}


void EndGameLayer::initPanel()
{
    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCPoint center = ccp(origin.x + vs.width / 2, origin.y + vs.height / 2);

    m_overlay = CCLayerColor::create(ccc4(0, 0, 0, 180));
    m_overlay->setContentSize(vs);
    m_overlay->setPosition(origin);
    this->addChild(m_overlay, 0);


    m_panelContainer = CCNode::create();
    this->addChild(m_panelContainer, 1);

    m_bgPanel = CCScale9Sprite::create("OD_Scale9Sprite.png");
    m_bgPanel->setContentSize(CCSizeMake(vs.width * 0.95f, vs.height * 0.4f));
    m_bgPanel->setAnchorPoint(ccp(0.5f, 0.5f));
    m_bgPanel->setPosition(center);
    m_panelContainer->addChild(m_bgPanel);

    showLayer();
}

bool EndGameLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    return true;
}

void EndGameLayer::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
}

void EndGameLayer::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
}

void EndGameLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -500, true);
}

std::string EndGameLayer::randomLabel()
{
    if (m_score >= m_bestScore && m_score > 0)
    {
        // new best
        int choice = rand() % 6;
        switch (choice)
        {
        case 0: return "AMAZING!";
        case 1: return "LEGENDARY!";
        case 2: return "UNSTOPPABLE!";
        case 3: return "PHENOMENAL!";
        case 4: return "INCREDIBLE!";
        case 5: return "PERFECT!";
        }
    }
    else if (m_score >= m_bestScore * 0.8f)
    {
        int choice = rand() % 5;
        switch (choice)
        {
        case 0: return "So Close!";
        case 1: return "Almost There!";
        case 2: return "Keep Pushing!";
        case 3: return "Nearly Perfect!";
        case 4: return "One More Try!";
        }
    }
    else if (m_score >= m_bestScore * 0.5f)
    {
        int choice = rand() % 5;
        switch (choice)
        {
        case 0: return "Good Try!";
        case 1: return "Keep Going!";
        case 2: return "You Can Do It!";
        case 3: return "Nice Effort!";
        case 4: return "Try Again!";
        }
    }
    else
    {
        int choice = rand() % 5;
        switch (choice)
        {
        case 0: return "Don't Give Up!";
        case 1: return "Practice Makes Perfect!";
        case 2: return "You'll Get Better!";
        case 3: return "Keep Trying!";
        case 4: return "Every Attempt Counts!";
        }
    }

    return "Try Again!";
}

void EndGameLayer::showLayer()
{
    if (!m_bgPanel) return;

    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCPoint center = ccp(origin.x + vs.width / 2, origin.y + vs.height / 2);

    CCPoint finalPos = center;

    m_bgPanel->setPosition(ccp(finalPos.x, finalPos.y + vs.height));
    m_bgPanel->setScale(1.f);

    CCMoveTo* moveToCenter = CCMoveTo::create(0.4f, finalPos);
    CCEaseBackOut* ease = CCEaseBackOut::create(moveToCenter);

    m_bgPanel->runAction(ease);
}

void EndGameLayer::initContent()
{
    CCSize panelSize = m_bgPanel->getContentSize();
    if (panelSize.width <= 0.f || panelSize.height <= 0.f) return;

    float padding = panelSize.height * 0.05f;
    float titleFontSize = panelSize.height * 0.08f;
    float labelFontSize = panelSize.height * 0.045f;
    float smallFontSize = panelSize.height * 0.03f;
    float buttonY = panelSize.height * 0.1f;

    std::string splashMsg = randomLabel();

    CCLabelBMFont* splashLabel = CCLabelBMFont::create(splashMsg.c_str(), "fonts/BigFont-Gold.fnt");
    splashLabel->setPosition(ccp(panelSize.width / 2, panelSize.height / 2 * 0.7f));
    splashLabel->setScale(1.f);

    float maxWidth = panelSize.width * 0.8f;
    splashLabel->setWidth(maxWidth);
    splashLabel->setAlignment(kCCTextAlignmentCenter);

    splashLabel->setColor(ccc3(255, 255, 255));
    m_bgPanel->addChild(splashLabel, 3);

    splashLabel->setOpacity(0);
    splashLabel->runAction(CCSequence::create(
        CCDelayTime::create(0.3f),
        CCFadeIn::create(0.3f),
        NULL
    ));

    CCSprite* gameOver = CCSprite::create("OD_GameOverLabel.png");
    gameOver->setPosition(ccp(panelSize.width / 2, panelSize.height / 2 * 2.f));
    gameOver->setScale(0.9f);
    m_bgPanel->addChild(gameOver, 2);

    CCScale9Sprite* second = CCScale9Sprite::create("OD_Scale9SpriteOrder.png");
    second->setContentSize(CCSizeMake(panelSize.width * 0.45f, panelSize.height * 0.30f));
    second->setAnchorPoint(ccp(0.5f, 0.5f));
    second->setPosition(ccp(panelSize.width / 2, panelSize.height / 2 * 1.3f));
    m_bgPanel->addChild(second, 2);

    CCSize secondSize = second->getContentSize();

    bool isNewRecord = (m_score >= m_bestScore && m_score > 0);

    if (isNewRecord) {
        CCAudioManager::sharedManager()->playSoundEffect("zapsplat_multimedia_ui_positive_tone_plucked_warm_positive_success_97094.mp3");
    }

    CCLabelTTF* scoreLabel = CCLabelTTF::create("SCORE", "fonts/Audiowide.ttf", secondSize.height * 0.18f);
    scoreLabel->setPosition(ccp(secondSize.width / 2, secondSize.height * 0.70f));
    scoreLabel->setColor(ccc3(200, 200, 200));
    second->addChild(scoreLabel);

    // Convertir int a string para 2.1.5
    std::ostringstream scoreStr;
    scoreStr << m_score;

    CCLabelTTF* scoreValue = CCLabelTTF::create(
        scoreStr.str().c_str(),
        "fonts/Audiowide.ttf",
        secondSize.height * 0.35f
    );
    scoreValue->setPosition(ccp(secondSize.width / 2, secondSize.height * 0.45f));

    if (isNewRecord)
    {
        scoreValue->setColor(ccc3(255, 215, 0));
        scoreValue->runAction(CCRepeatForever::create(
            CCSequence::create(
                CCTintTo::create(0.5f, 255, 255, 150),
                CCTintTo::create(0.5f, 255, 215, 0),
                NULL
            )
        ));

        CCLabelTTF* newRecordLabel = CCLabelTTF::create("NEW RECORD!", "fonts/Audiowide.ttf", secondSize.height * 0.12f);
        newRecordLabel->setPosition(ccp(secondSize.width / 2, secondSize.height * 0.90f));
        newRecordLabel->setColor(ccc3(255, 215, 0));
        second->addChild(newRecordLabel);

        newRecordLabel->runAction(CCRepeatForever::create(
            CCSequence::create(
                CCScaleTo::create(0.3f, 1.1f),
                CCScaleTo::create(0.3f, 1.0f),
                NULL
            )
        ));
    }
    else
    {
        scoreValue->setColor(ccc3(255, 255, 255));
    }

    second->addChild(scoreValue);

    CCLabelTTF* bestLabel = CCLabelTTF::create("BEST", "fonts/Audiowide.ttf", secondSize.height * 0.14f);
    bestLabel->setPosition(ccp(secondSize.width / 2, secondSize.height * 0.20f));
    bestLabel->setColor(ccc3(180, 180, 180));
    second->addChild(bestLabel);

    std::ostringstream bestStr;
    bestStr << m_bestScore;

    CCLabelTTF* bestValue = CCLabelTTF::create(
        bestStr.str().c_str(),
        "fonts/Audiowide.ttf",
        secondSize.height * 0.20f
    );
    bestValue->setPosition(ccp(secondSize.width / 2, secondSize.height * 0.02f));
    bestValue->setColor(ccc3(200, 200, 200));
    second->addChild(bestValue);

    CCSprite* retry = CCSprite::create("OD_EndGameRetry.png");
    CCMenuItemSpriteExtra* retryBtn = CCMenuItemSpriteExtra::create(
        retry,
        this,
        menu_selector(EndGameLayer::onRetryClicked)
    );

    retryBtn->setPosition(ccp(panelSize.width / 2 * 0.4f, panelSize.height / 2 * 0.1f));
    retryBtn->setScale(1.2f);
    retryBtn->setScaleMultiplier(1.2f);

    CCSprite* home = CCSprite::create("OD_EndGameHome.png");
    CCMenuItemSpriteExtra* homeBtn = CCMenuItemSpriteExtra::create(
        home,
        this,
        menu_selector(EndGameLayer::onHomeClicked)
    );

    homeBtn->setPosition(ccp(panelSize.width / 2 * 1.5f, panelSize.height / 2 * 0.1f));
    homeBtn->setScale(1.2f);
    homeBtn->setScaleMultiplier(1.2f);

    CCMenu* menu = CCMenu::create(retryBtn, homeBtn, NULL);
    menu->setPosition(CCPointZero);
    menu->setTouchPriority(-501);
    m_bgPanel->addChild(menu, 2);
}

void EndGameLayer::onRetryClicked(CCObject* sender)
{
    CCAudioManager::sharedManager()->stopBgMusic();
    CCScene* newPlayScene = PlayScene::createScene();
    CCDirector::sharedDirector()->replaceScene(
        CCTransitionFade::create(0.5f, newPlayScene)
    );
}

void EndGameLayer::onHomeClicked(CCObject* sender)
{
    CCAudioManager::sharedManager()->stopBgMusic();
    CCAudioManager::sharedManager()->playBgMusic("MenuLoop.mp3", true);
    CCScene* menuScene = MenuScene::createScene();
    CCDirector::sharedDirector()->replaceScene(
        CCTransitionFade::create(0.5f, menuScene)
    );
}

void EndGameLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayerColor::onExit();
}