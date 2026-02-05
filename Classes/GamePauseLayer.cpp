#include "GamePauseLayer.h"
#include "CCMenuItemSpriteExtra.h"
#include "PlayScene.h"
#include "MenuScene.h"
#include "RingNode.h"
#include "SettingsLayer.h"
#include "PlayerObject.h"
#include "CCAudioManager.h"

USING_NS_CC;

GamePauseLayer* GamePauseLayer::create()
{
	GamePauseLayer* layer = new GamePauseLayer();
	if (layer && layer->init()) {
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}

void GamePauseLayer::onEnter()
{
    CCLayerColor::onEnter();
    this->registerWithTouchDispatcher();
}

bool GamePauseLayer::init()
{
	if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0))) return false;

	initPause();
	initButtons();

	CCAudioManager::sharedManager()->pauseBgMusic();

	return true;
}

bool GamePauseLayer::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    return true;
}

void GamePauseLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -500, true);
}

void GamePauseLayer::initPause()
{
	CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	m_overlay = CCLayerColor::create(ccc4(0, 0, 0, 180));
	m_overlay->setContentSize(vs);
	m_overlay->setPosition(origin);
	this->addChild(m_overlay, 0);

	m_panelContainer = CCNode::create();
	this->addChild(m_panelContainer, 1);
}

void GamePauseLayer::initButtons()
{
    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCPoint center = ccp(origin.x + vs.width / 2, origin.y + vs.height / 2);

    CCSprite* resume = CCSprite::create("OD_ResumeButton.png");
    CCMenuItemSpriteExtra* resumeItem = CCMenuItemSpriteExtra::create(
        resume,
        this,
        menu_selector(GamePauseLayer::onResumeClicked)
    );
    resumeItem->setPosition(ccp(center.x, center.y * 1.4f));

    CCSprite* retry = CCSprite::create("OD_RetryButton.png");
    CCMenuItemSpriteExtra* retryItem = CCMenuItemSpriteExtra::create(
        retry,
        this,
        menu_selector(GamePauseLayer::onRetryClicked)
    );
    retryItem->setPosition(ccp(center.x, center.y * 1.2f));

    CCSprite* settings = CCSprite::create("OD_PauseSettingsButton.png");
    CCMenuItemSpriteExtra* settingsItem = CCMenuItemSpriteExtra::create(
        settings,
        this,
        menu_selector(GamePauseLayer::onSettingsClicked)
    );
    settingsItem->setPosition(ccp(center.x, center.y));

    CCSprite* exit = CCSprite::create("OD_ExitButton.png");
    CCMenuItemSpriteExtra* exitItem = CCMenuItemSpriteExtra::create(
        exit,
        this,
        menu_selector(GamePauseLayer::onExitClicked)
    );
    exitItem->setPosition(ccp(center.x, center.y * 0.8f));

    CCMenu* menu = CCMenu::create(resumeItem, retryItem, settingsItem, exitItem, NULL);
    menu->setPosition(CCPointZero);
    menu->setTouchPriority(-501);
    m_panelContainer->addChild(menu, 2);
}

void GamePauseLayer::onResumeClicked(CCObject* sender)
{
    PlayScene* playScene = dynamic_cast<PlayScene*>(this->getParent());
    if (playScene) {
        playScene->scheduleUpdate();

        for (size_t i = 0; i < playScene->m_obstacles.size(); i++) {
            playScene->m_obstacles[i]->resumeSchedulerAndActions();
        }

        if (playScene->m_ring) {
            playScene->m_ring->resumeSchedulerAndActions();
        }

        if (playScene->m_player) {
            playScene->m_player->resumeSchedulerAndActions();
        }
    }

    CCAudioManager::sharedManager()->resumeBgMusic();
    this->removeFromParentAndCleanup(true);
}

void GamePauseLayer::onRetryClicked(CCObject* sender)
{
    CCDirector::sharedDirector()->resume();
    CCAudioManager::sharedManager()->stopBgMusic();

    CCScene* scene = PlayScene::createScene();
    CCDirector::sharedDirector()->replaceScene(scene);
}

void GamePauseLayer::onSettingsClicked(CCObject* sender)
{
   SettingsLayer* layer = SettingsLayer::create();
    this->addChild(layer, 2);
}

void GamePauseLayer::onExitClicked(CCObject* sender)
{
    CCDirector::sharedDirector()->resume();
    CCAudioManager::sharedManager()->stopBgMusic();
    CCAudioManager::sharedManager()->playBgMusic("MenuLoop.mp3", true);

    CCScene* scene = MenuScene::createScene();
    CCDirector::sharedDirector()->replaceScene(scene);
}

void GamePauseLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayerColor::onExit();
}