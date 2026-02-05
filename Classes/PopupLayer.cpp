#include "PopupLayer.h"
#include "CCMenuItemSpriteExtra.h"

USING_NS_CC;

PopupLayer* PopupLayer::create()
{
    PopupLayer* layer = new PopupLayer();
    if (layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

void PopupLayer::onEnter()
{
    CCLayerColor::onEnter();
    this->registerWithTouchDispatcher();
}


bool PopupLayer::init()
{
    if (!CCLayerColor::init()) return false;

    m_bgPanel = NULL;
    m_panelContainer = NULL;
    m_overlay = NULL;

    initPopup();
    initContent();

    return true;
}

bool PopupLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    return true;
}

void PopupLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{

}

void PopupLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -500, true);
}

void PopupLayer::initPopup()
{
    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCPoint center = ccp(origin.x + vs.width / 2, origin.y + vs.height / 2);

    m_overlay = CCLayerColor::create(ccc4(0, 0, 0, 180));
    m_overlay->setContentSize(vs);
    m_overlay->setPosition(origin);
    m_overlay->setOpacity(0);
    this->addChild(m_overlay, 0);

    m_panelContainer = CCNode::create();
    this->addChild(m_panelContainer, 1);

    showLayer();
}

void PopupLayer::showLayer()
{
    m_overlay->runAction(CCFadeTo::create(0.3f, 180));
}

void PopupLayer::initContent()
{
    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCPoint center = ccp(origin.x + vs.width / 2, origin.y + vs.height / 2);

    CCSprite* coming = CCSprite::create("OD_ComingSoonLabel.png");
    coming->setPosition(center);
    coming->setOpacity(0);
    coming->setScale(0.8f);

    CCFadeIn* fadeInComing = CCFadeIn::create(0.4f);
    CCScaleTo* scaleUpComing = CCScaleTo::create(0.4f, 1.0f);
    CCSpawn* spawnComing = CCSpawn::create(fadeInComing, scaleUpComing, NULL);
    CCEaseBackOut* easeComing = CCEaseBackOut::create(spawnComing);

    coming->runAction(CCSequence::create(
        CCDelayTime::create(0.2f),
        easeComing,
        NULL
    ));

    m_panelContainer->addChild(coming, 2);

    CCSprite* ok = CCSprite::create("OD_OkButton.png");
    CCMenuItemSpriteExtra* okBtn = CCMenuItemSpriteExtra::create(
        ok,
        this,
        menu_selector(PopupLayer::onCloseButtonClicked)
    );
    okBtn->setPosition(ccp(center.x, center.y * 0.8f));
    okBtn->setScale(1.f);
    okBtn->setOpacity(0);

    CCFadeIn* fadeInBtn = CCFadeIn::create(0.3f);
    CCSequence* scaleBtn = CCSequence::create(
        CCScaleTo::create(0.15f, 1.2f),
        CCScaleTo::create(0.15f, 1.0f),
        NULL
    );
    CCSpawn* spawnBtn = CCSpawn::create(fadeInBtn, scaleBtn, NULL);

    okBtn->runAction(CCSequence::create(
        CCDelayTime::create(0.5f),
        spawnBtn,
        NULL
    ));

    CCMenu* menu = CCMenu::create(okBtn, NULL);
    menu->setPosition(CCPointZero);
    menu->setTouchPriority(-501);
    m_panelContainer->addChild(menu, 2);
}

void PopupLayer::onCloseButtonClicked(CCObject* sender)
{
    this->removeFromParent();
}

void PopupLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayerColor::onExit();
}