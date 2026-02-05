#include "MenuScene.h"
#include "CCMenuItemSpriteExtra.h"
#include "PlayScene.h"
#include "PopupLayer.h"
#include "SupportLayer.h"
#include "SettingsLayer.h"
#include "CCAudioManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>
#include <shellapi.h>
#endif

USING_NS_CC;

CCScene* MenuScene::createScene()
{
    CCScene* scene = CCScene::create();
    MenuScene* layer = new MenuScene();
    if (layer && layer->init()) {
        layer->autorelease();
        scene->addChild(layer);
    }
    else {
        CC_SAFE_DELETE(layer);
    }

    return scene;
}

bool MenuScene::init()
{
    if (!CCLayer::init()) return false;

    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCAudioManager::sharedManager()->playBgMusic("MenuLoop.mp3", true);

    CCSprite* bg = CCSprite::create("OD_GameBackground.png");
    bg->setContentSize(vs);
    bg->setPosition(ccp(origin.x + vs.width / 2, origin.y + vs.height / 2));
    this->addChild(bg, 0);

    CCSprite* logo = CCSprite::create("OD_MainSpriteLogo.png");
    logo->setPosition(ccp(origin.x + vs.width / 2, origin.y + vs.height / 2 * 1.6f));
    logo->setScale(0.6f);
    addChild(logo);

    CCSprite* play = CCSprite::create("OD_PlayButton.png");
    CCMenuItemSpriteExtra* playBtn = CCMenuItemSpriteExtra::create(
        play,
        this,
        menu_selector(MenuScene::onPlayClicked)
    );
    playBtn->setPosition(ccp(origin.x + vs.width / 2, origin.y + vs.height / 2 * 1.1f));
    playBtn->setScale(1.0f);
    playBtn->setScaleMultiplier(1.2f);

    CCSprite* customize = CCSprite::create("OD_CustomButton.png");
    CCMenuItemSpriteExtra* customizeBtn = CCMenuItemSpriteExtra::create(
        customize,
        this,
        menu_selector(MenuScene::onCustomizeClicked)
    );
    customizeBtn->setPosition(ccp(origin.x + vs.width / 2, origin.y + vs.height / 2 * 0.9f));
    customizeBtn->setScale(1.0f);
    customizeBtn->setScaleMultiplier(1.2f);

    CCSprite* leaderboard = CCSprite::create("OD_LeaderboardButton.png");
    CCMenuItemSpriteExtra* leaderboardBtn = CCMenuItemSpriteExtra::create(
        leaderboard,
        this,
        menu_selector(MenuScene::onLeaderboardClicked)
    );
    leaderboardBtn->setPosition(ccp(origin.x + vs.width / 2, origin.y + vs.height / 2 * 0.7f));
    leaderboardBtn->setScale(1.0f);
    leaderboardBtn->setScaleMultiplier(1.2f);

    playBtn->setOpacity(0);
    playBtn->setPositionY(playBtn->getPositionY() - 20);
    playBtn->runAction(
        CCSpawn::create(
            CCFadeIn::create(0.4f),
            CCEaseBackOut::create(CCMoveBy::create(0.4f, ccp(0, 20))),
            NULL
        )
    );

    customizeBtn->setOpacity(0);
    customizeBtn->setPositionY(customizeBtn->getPositionY() - 20);
    customizeBtn->runAction(
        CCSequence::create(
            CCDelayTime::create(0.1f),
            CCSpawn::create(
                CCFadeIn::create(0.4f),
                CCEaseBackOut::create(CCMoveBy::create(0.4f, ccp(0, 20))),
                NULL
            ),
            NULL
        )
    );

    leaderboardBtn->setOpacity(0);
    leaderboardBtn->setPositionY(leaderboardBtn->getPositionY() - 20);
    leaderboardBtn->runAction(
        CCSequence::create(
            CCDelayTime::create(0.2f),
            CCSpawn::create(
                CCFadeIn::create(0.4f),
                CCEaseBackOut::create(CCMoveBy::create(0.4f, ccp(0, 20))),
                NULL
            ),
            NULL
        )
    );

    CCMenu* mainMenu = CCMenu::create(playBtn, customizeBtn, leaderboardBtn, NULL);
    mainMenu->setPosition(CCPointZero);
    addChild(mainMenu);

    CCSprite* about = CCSprite::create("OD_AboutButton.png");
    CCMenuItemSpriteExtra* aboutBtn = CCMenuItemSpriteExtra::create(
        about,
        this,
        menu_selector(MenuScene::onAboutClicked)
    );
    aboutBtn->setPosition(ccp(origin.x + vs.width / 2 * 0.4f, origin.y + vs.height / 2 * 0.2f));
    aboutBtn->setScale(1.0f);
    aboutBtn->setScaleMultiplier(1.2f);

    CCSprite* settings = CCSprite::create("OD_SettingsButton.png");
    CCMenuItemSpriteExtra* settingsBtn = CCMenuItemSpriteExtra::create(
        settings,
        this,
        menu_selector(MenuScene::onSettingsClicked)
    );
    settingsBtn->setPosition(ccp(origin.x + vs.width / 2, origin.y + vs.height / 2 * 0.2f));
    settingsBtn->setScale(1.0f);
    settingsBtn->setScaleMultiplier(1.2f);

    CCSprite* music = CCSprite::create("OD_MusicButton.png");
    CCMenuItemSpriteExtra* musicBtn = CCMenuItemSpriteExtra::create(
        music,
        this,
        menu_selector(MenuScene::onMusicClicked)
    );
    musicBtn->setPosition(ccp(origin.x + vs.width / 2 * 1.6, origin.y + vs.height / 2 * 0.2f));
    musicBtn->setScale(1.0f);
    musicBtn->setScaleMultiplier(1.2f);

    aboutBtn->setOpacity(0);
    aboutBtn->setPositionY(aboutBtn->getPositionY() - 15);
    aboutBtn->runAction(
        CCSequence::create(
            CCDelayTime::create(0.3f),
            CCSpawn::create(
                CCFadeIn::create(0.4f),
                CCEaseBackOut::create(CCMoveBy::create(0.4f, ccp(0, 15))),
                NULL
            ),
            NULL
        )
    );

    settingsBtn->setOpacity(0);
    settingsBtn->setPositionY(settingsBtn->getPositionY() - 15);
    settingsBtn->runAction(
        CCSequence::create(
            CCDelayTime::create(0.4f),
            CCSpawn::create(
                CCFadeIn::create(0.4f),
                CCEaseBackOut::create(CCMoveBy::create(0.4f, ccp(0, 15))),
                NULL
            ),
            NULL
        )
    );

    musicBtn->setOpacity(0);
    musicBtn->setPositionY(musicBtn->getPositionY() - 15);
    musicBtn->runAction(
        CCSequence::create(
            CCDelayTime::create(0.5f),
            CCSpawn::create(
                CCFadeIn::create(0.4f),
                CCEaseBackOut::create(CCMoveBy::create(0.4f, ccp(0, 15))),
                NULL
            ),
            NULL
        )
    );

    CCMenu* subMenu = CCMenu::create(aboutBtn, settingsBtn, musicBtn, NULL);
   // subMenu->setIgnoreAnchorPointForPosition(false);
    subMenu->setPosition(CCPointZero);
    this->addChild(subMenu, 2);

    return true;
}

void MenuScene::onPlayClicked(CCObject* sender)
{
    CCScene* scene = PlayScene::createScene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
}

void MenuScene::onCustomizeClicked(CCObject* sender)
{
   PopupLayer* layer = PopupLayer::create();
   this->addChild(layer, 2);
}

void MenuScene::onLeaderboardClicked(CCObject* sender)
{
   PopupLayer* layer = PopupLayer::create();
    this->addChild(layer, 2);
}

void MenuScene::onAboutClicked(CCObject* sender)
{
   SupportLayer* layer = SupportLayer::create();
   this->addChild(layer, 10);
}

void MenuScene::onSettingsClicked(CCObject* sender)
{
   SettingsLayer* layer = SettingsLayer::create();
    this->addChild(layer, 10);
}

void MenuScene::onMusicClicked(CCObject* sender)
{
    const char* url = "https://urafaelgames.com/songs-credits";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
#endif
}