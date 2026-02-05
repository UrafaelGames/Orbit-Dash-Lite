#include "LoadingScene.h"
#include "MenuScene.h"
#include <ctime>
#include <cstdlib>

USING_NS_CC;

std::string LoadingScene::getLoadSplash()
{
    srand(static_cast<unsigned int>(time(NULL)));
    int splashes = rand() % 6;

    switch (splashes)
    {
    case 0: return "Reticulating splines..."; break;
    case 1: return "This splash was approved by Urafael"; break;
    case 2: return "Practicing perfect timing..."; break;
    case 3: return "Well"; break;
    case 4: return "Urafael is love, Urafael is life"; break;
    case 5: return "What are ya?"; break;
    default: return "Loading..."; break;
    }
}

CCScene* LoadingScene::createScene()
{
    CCScene* scene = CCScene::create();
    LoadingScene* layer = new LoadingScene();
    if (layer && layer->init())
    {
        layer->autorelease();
        scene->addChild(layer);
    }
    else
    {
        CC_SAFE_DELETE(layer);
    }
    return scene;
}

bool LoadingScene::init()
{
    if (!CCLayer::init()) return false;

    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCLOG("End");

    std::string splash = getLoadSplash();

    CCSprite* bg = CCSprite::create("OD_GameBackground.png");
    bg->setContentSize(vs);
    bg->setPosition(ccp(origin.x + vs.width / 2, origin.y + vs.height / 2));
    this->addChild(bg, 0);

    CCSprite* logo = CCSprite::create("UrafaelLogo.png");
    logo->setPosition(ccp(origin.x + vs.width / 2, origin.y + vs.height / 2));
    logo->setScale(0.f);
    logo->setOpacity(0);
    addChild(logo);

    CCLabelBMFont* label = CCLabelBMFont::create(splash.c_str(), "fonts/BigFont-Gold.fnt");
    label->setPosition(ccp(origin.x + vs.width / 2, origin.y + vs.height / 2 * 0.6f));
    label->setScale(0.8f);
    label->setAnchorPoint(ccp(0.5f, 0.5f));
    if (splash.length() > 20) {
        label->setScale(0.6f);
    }

    addChild(label);

    CCFadeIn* fadeIn = CCFadeIn::create(0.5f);
    CCEaseBackOut* scaleUp = CCEaseBackOut::create(CCScaleTo::create(0.8f, 1.5f));

    CCSpawn* appear = CCSpawn::create(fadeIn, scaleUp, NULL);

    CCDelayTime* delay = CCDelayTime::create(0.7f);

    CCFadeOut* fO = CCFadeOut::create(0.8f);
    CCEaseBackIn* scaleD = CCEaseBackIn::create(CCScaleTo::create(1.5f, 0.0f));

    CCSpawn* disappear = CCSpawn::create(fO, scaleD, NULL);

    CCCallFunc* callback = CCCallFunc::create(this, callfunc_selector(LoadingScene::onLogoAnimationComplete));

    CCSequence* seq = CCSequence::create(appear, delay, disappear, callback, NULL);

    logo->runAction(seq);

    CCSprite* cocos = CCSprite::create("cocos2dLogo.png");
    cocos->setPosition(ccp(origin.x + vs.width / 2 * 1.5f, origin.y + vs.height / 2 * 0.07f));
    cocos->setScale(1.f);
    addChild(cocos);

    return true;
}

void LoadingScene::onLogoAnimationComplete()
{
    CCScene* scene = MenuScene::createScene();
     CCDirector::sharedDirector()->replaceScene(scene);
}