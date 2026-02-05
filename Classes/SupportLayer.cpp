#include "SupportLayer.h"
#include "CCMenuItemSpriteExtra.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>
#include <shellapi.h>
#endif

USING_NS_CC;
USING_NS_CC_EXT;

SupportLayer* SupportLayer::create()
{
    SupportLayer* layer = new SupportLayer();
    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

void SupportLayer::onEnter()
{
    CCLayerColor::onEnter();
    this->registerWithTouchDispatcher();
}

bool SupportLayer::init()
{
    if (!CCLayerColor::init()) return false;

    m_bgPanel = NULL;
    m_panelContainer = NULL;
    m_overlay = NULL;

    this->setTouchPriority(kCCMenuHandlerPriority - 1);

    initPanel();
    initCloseButton();
    initContent();

    return true;
}

bool SupportLayer::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{

    return true;
}

void SupportLayer::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
}

void SupportLayer::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
}

void SupportLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -500, true);
}

void SupportLayer::initPanel()
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
    m_bgPanel->setContentSize(CCSizeMake(vs.width * 0.9f, vs.height * 0.4f));
    m_bgPanel->setAnchorPoint(ccp(0.5f, 0.5f));
    m_bgPanel->setPosition(center);
    m_panelContainer->addChild(m_bgPanel);

    showLayer();
}

void SupportLayer::showLayer()
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

void SupportLayer::initCloseButton()
{
    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCPoint center = ccp(origin.x + vs.width / 2, origin.y + vs.height / 2);

    CCSprite* arrow = CCSprite::create("OD_BackButton.png");

    CCMenuItemSpriteExtra* closeBtn = CCMenuItemSpriteExtra::create(
        arrow,
        this,
        menu_selector(SupportLayer::onCloseClicked)
    );

    float panelW = m_bgPanel->getContentSize().width;
    float panelH = m_bgPanel->getContentSize().height;

    closeBtn->setPosition(ccp(panelW / 2 * 0.1f, panelH / 2 * 1.85f));
    closeBtn->setScale(1.f);
    closeBtn->setScaleMultiplier(1.2f);

    CCMenu* menu = CCMenu::create(closeBtn, NULL);
    menu->setPosition(CCPointZero);
    m_bgPanel->addChild(menu, 1);
}

void SupportLayer::onCloseClicked(CCObject* sender)
{
    this->removeFromParent();
}

void SupportLayer::initContent()
{
    CCSize panelSize = m_bgPanel->getContentSize();
    if (panelSize.width <= 0.f || panelSize.height <= 0.f) return;

    float padding = panelSize.height * 0.05f;
    float titleFontSize = panelSize.height * 0.08f;
    float labelFontSize = panelSize.height * 0.045f;
    float smallFontSize = panelSize.height * 0.03f;
    float buttonY = panelSize.height * 0.1f;

    CCLabelBMFont* support = CCLabelBMFont::create("Support", "fonts/BigFont-Gold.fnt");
    support->setPosition(ccp(panelSize.width / 2, panelSize.height / 2 * 1.9f));
    support->setScale(1.1f);
    m_bgPanel->addChild(support, 2);

    CCLabelTTF* label = CCLabelTTF::create(
        "Send us an email at: support@urafaelgames.com",
        "fonts/arial.ttf",
        24
    );
    label->setPosition(ccp(panelSize.width / 2, panelSize.height / 2 * 1.6f));
    label->setScale(1.5f);
    m_bgPanel->addChild(label, 2);

    CCScale9Sprite* second = CCScale9Sprite::create("OD_Scale9SpriteOrder.png");
    second->setContentSize(CCSizeMake(panelSize.width * 0.95f, panelSize.height * 0.70f));
    second->setAnchorPoint(ccp(0.5f, 0.5f));
    second->setPosition(ccp(panelSize.width / 2, panelSize.height / 2 * 0.8f));
    m_bgPanel->addChild(second, 2);

    CCLabelBMFont* supportLbl = CCLabelBMFont::create("Developed By: ", "fonts/BigFont.fnt");
    supportLbl->setPosition(ccp(panelSize.width / 2 * 0.6f, panelSize.height / 2 * 1.2f));
    supportLbl->setScale(0.6f);
    m_bgPanel->addChild(supportLbl, 2);

    CCSprite* urafael = CCSprite::create("UrafaelLogo.png");
    CCMenuItemSpriteExtra* uraSpr = CCMenuItemSpriteExtra::create(
        urafael,
        this,
        menu_selector(SupportLayer::onUrafaelClicked)
    );

    uraSpr->setAnchorPoint(ccp(0.5f, 0.5f));
    uraSpr->setPosition(ccp(panelSize.width / 2 * 1.4f, panelSize.height / 2 * 0.95f));
    uraSpr->setScale(1.2f);
    uraSpr->setScaleMultiplier(1.2f);

    CCMenu* menu = CCMenu::create(uraSpr, NULL);
    menu->setPosition(CCPointZero);
    menu->setTouchPriority(-501);
    m_bgPanel->addChild(menu, 2);

    CCLabelBMFont* poweredLbl = CCLabelBMFont::create("Powered By: ", "fonts/BigFont.fnt");
    poweredLbl->setPosition(ccp(panelSize.width / 2 * 0.6f, panelSize.height / 2 * 0.45f));
    poweredLbl->setScale(0.6f);
    m_bgPanel->addChild(poweredLbl, 2);

    CCSprite* cocos = CCSprite::create("cocos2dLogo.png");
    CCMenuItemSpriteExtra* cocosSpr = CCMenuItemSpriteExtra::create(
        cocos,
        this,
        menu_selector(SupportLayer::onCocosClicked)
    );

    cocosSpr->setPosition(ccp(panelSize.width / 2 * 1.4f, panelSize.height / 2 * 0.4f));
    cocosSpr->setScale(0.9f);
    cocosSpr->setScaleMultiplier(1.2f);


    CCMenu* menu2 = CCMenu::create(cocosSpr, NULL);
    menu2->setPosition(CCPointZero);
    menu2->setTouchPriority(-501);
    m_bgPanel->addChild(menu2, 2);

    CCLabelBMFont* version = CCLabelBMFont::create("Version: 1.0", "fonts/BigFont-Gold.fnt");
    version->setPosition(ccp(panelSize.width / 2 * 1.7f, panelSize.height / 2 * 0.1f));
    version->setScale(0.5f);
    m_bgPanel->addChild(version, 2);
}

void SupportLayer::onUrafaelClicked(CCObject* sender)
{
    const char* url = "https://urafaelgames.com";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
#endif
}

void SupportLayer::onCocosClicked(CCObject* sender)
{
    const char* url = "https://www.cocos.com/en/cocos2d-x";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
#endif
}


void SupportLayer::onExit() 
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayerColor::onExit();
}

