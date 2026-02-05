#include "SettingsLayer.h"
#include "CCMenuItemSpriteExtra.h"
#include "CCGameManager.h"
#include <sstream>

USING_NS_CC;
USING_NS_CC_EXT;

SettingsLayer* SettingsLayer::create()
{
    SettingsLayer* layer = new SettingsLayer();
    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

void SettingsLayer::onEnter()
{
    CCLayerColor::onEnter();
    this->registerWithTouchDispatcher();
}

bool SettingsLayer::init()
{
    if (!CCLayerColor::init()) return false;

    m_bgPanel = NULL;
    m_panelContainer = NULL;
    m_overlay = NULL;
    m_bgVolumeLabel = NULL;
    m_sfxVolumeLabel = NULL;
    m_bgVolume = NULL;
    m_sfxVolume = NULL;

    initPanel();
    initCloseButton();
    initContent();

    return true;
}

bool SettingsLayer::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    return true;
}

void SettingsLayer::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
}

void SettingsLayer::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
}

void SettingsLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -500, true);
}

void SettingsLayer::initPanel()
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

void SettingsLayer::showLayer()
{
    if (!m_bgPanel) return;

    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCPoint center = ccp(origin.x + vs.width / 2, origin.y + vs.height / 2);

    m_bgPanel->setPosition(center);
    m_bgPanel->setScale(0.f);

    CCScaleTo* scaleUp = CCScaleTo::create(0.15f, 1.15f);
    CCScaleTo* scaleBounce = CCScaleTo::create(0.1f, 1.f);

    CCSequence* bounce = CCSequence::create(scaleUp, scaleBounce, NULL);

    m_bgPanel->runAction(bounce);
}

void SettingsLayer::initCloseButton()
{
    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCPoint center = ccp(origin.x + vs.width / 2, origin.y + vs.height / 2);

    CCSprite* arrow = CCSprite::create("OD_BackButton.png");

    CCMenuItemSpriteExtra* closeBtn = CCMenuItemSpriteExtra::create(
        arrow,
        this,
        menu_selector(SettingsLayer::onCloseClicked)
    );

    float panelW = m_bgPanel->getContentSize().width;
    float panelH = m_bgPanel->getContentSize().height;

    closeBtn->setPosition(ccp(panelW / 2 * 0.1f, panelH / 2 * 1.85f));
    closeBtn->setScale(1.f);
    closeBtn->setScaleMultiplier(1.2f);

    CCMenu* menu = CCMenu::create(closeBtn, NULL);
    menu->setPosition(CCPointZero);
    menu->setTouchPriority(-501);
    m_bgPanel->addChild(menu, 1);
}

void SettingsLayer::onCloseClicked(CCObject* sender)
{
    this->removeFromParent();
}

void SettingsLayer::initContent()
{
    CCSize panelSize = m_bgPanel->getContentSize();
    if (panelSize.width <= 0.f || panelSize.height <= 0.f) return;

    float sliderScale = 0.6f;

    CCLabelBMFont* settings = CCLabelBMFont::create("Settings", "fonts/BigFont-Gold.fnt");
    settings->setPosition(ccp(panelSize.width / 2, panelSize.height / 2 * 1.9f));
    settings->setScale(1.1f);
    m_bgPanel->addChild(settings, 2);

    // Music Slider
    int musicVolume = static_cast<int>(CCGameManager::sharedManager()->getMusicVolume() * 100);
    char musicStr[32];
    sprintf(musicStr, "Music: %d%%", musicVolume);

    m_bgVolumeLabel = CCLabelBMFont::create(musicStr, "fonts/BigFont.fnt");
    m_bgVolumeLabel->setAnchorPoint(ccp(0.5f, 0.f));
    m_bgVolumeLabel->setPosition(ccp(panelSize.width / 2, panelSize.height / 2 * 1.5f));
    m_bgVolumeLabel->setScale(0.7f);
    m_bgPanel->addChild(m_bgVolumeLabel, 1);

    CCSprite* bgSprite1 = CCSprite::create("OD_SliderBacground.png");
    CCSprite* progressSprite1 = CCSprite::create("OD_SliderField.png");
    CCSprite* thumbSprite1 = CCSprite::create("OD_SliderBallNormal.png");

    m_bgVolume = CCControlSlider::create(bgSprite1, progressSprite1, thumbSprite1);
    m_bgVolume->setAnchorPoint(ccp(0.5f, 0.5f));
    m_bgVolume->setPosition(ccp(panelSize.width / 2, panelSize.height / 2 * 1.2f));
    m_bgVolume->setScale(sliderScale);
    m_bgVolume->setMinimumValue(0.0f);
    m_bgVolume->setMaximumValue(100.0f);
    m_bgVolume->setValue(musicVolume);
    m_bgVolume->addTargetWithActionForControlEvents(
        this,
        cccontrol_selector(SettingsLayer::onMusicVolumeChanged),
        CCControlEventValueChanged
    );
    m_bgVolume->setTouchPriority(-501);
    m_bgPanel->addChild(m_bgVolume, 1);

    // SFX Slider
    int effectsVolume = static_cast<int>(CCGameManager::sharedManager()->getEffectsVolume() * 100);
    char effectsStr[32];
    sprintf(effectsStr, "SFX: %d%%", effectsVolume);

    m_sfxVolumeLabel = CCLabelBMFont::create(effectsStr, "fonts/BigFont.fnt");
    m_sfxVolumeLabel->setAnchorPoint(ccp(0.5f, 0.f));
    m_sfxVolumeLabel->setPosition(ccp(panelSize.width / 2, panelSize.height / 2 * 0.8f));
    m_sfxVolumeLabel->setScale(0.7f);
    m_bgPanel->addChild(m_sfxVolumeLabel, 1);

    CCSprite* bgSprite2 = CCSprite::create("OD_SliderBacground.png");
    CCSprite* progressSprite2 = CCSprite::create("OD_SliderField.png");
    CCSprite* thumbSprite2 = CCSprite::create("OD_SliderBallNormal.png");

    m_sfxVolume = CCControlSlider::create(bgSprite2, progressSprite2, thumbSprite2);
    m_sfxVolume->setAnchorPoint(ccp(0.5f, 0.5f));
    m_sfxVolume->setPosition(ccp(panelSize.width / 2, panelSize.height / 2 * 0.5f));
    m_sfxVolume->setScale(sliderScale);
    m_sfxVolume->setMinimumValue(0.0f);
    m_sfxVolume->setMaximumValue(100.0f);
    m_sfxVolume->setValue(effectsVolume);
    m_sfxVolume->addTargetWithActionForControlEvents(
        this,
        cccontrol_selector(SettingsLayer::onEffectsVolumeChanged),
        CCControlEventValueChanged
    );
    m_sfxVolume->setTouchPriority(-501);
    m_bgPanel->addChild(m_sfxVolume, 1);
}

void SettingsLayer::updateVolumeLabel()
{
    char musicStr[20];
    sprintf(musicStr, "Music: %d%%", (int)m_bgVolume->getValue());
    m_bgVolumeLabel->setString(musicStr);

    char effectsStr[20];
    sprintf(effectsStr, "SFX: %d%%", (int)m_sfxVolume->getValue());
    m_sfxVolumeLabel->setString(effectsStr);
}

void SettingsLayer::onMusicVolumeChanged(cocos2d::CCObject* sender, cocos2d::extension::CCControlEvent event)
{
    float volume = m_bgVolume->getValue() / 100.f;
    CCGameManager::sharedManager()->setMusicVolume(volume);
    CCGameManager::sharedManager()->saveData();
    updateVolumeLabel();

    CCSequence* scaleAction = CCSequence::create(
        CCScaleTo::create(0.1f, 0.8f),
        CCScaleTo::create(0.1f, 0.7f),
        NULL
    );
    m_bgVolumeLabel->runAction(scaleAction);
}

void SettingsLayer::onEffectsVolumeChanged(cocos2d::CCObject* sender, cocos2d::extension::CCControlEvent event)
{
    float volume = m_sfxVolume->getValue() / 100.f;
    CCGameManager::sharedManager()->setEffectsVolume(volume);
    CCGameManager::sharedManager()->saveData();
    updateVolumeLabel();

    CCSequence* scaleAction = CCSequence::create(
        CCScaleTo::create(0.1f, 0.8f),
        CCScaleTo::create(0.1f, 0.7f),
        NULL
    );
    m_sfxVolumeLabel->runAction(scaleAction);
}

void SettingsLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayerColor::onExit();
}