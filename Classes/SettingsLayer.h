#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"

class SettingsLayer : public cocos2d::CCLayerColor
{
public:
    static SettingsLayer* create();
    void onEnter();
    void onExit();
    virtual bool init();

    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void registerWithTouchDispatcher();

private:
    void showLayer();
    void initPanel();
    void initCloseButton();
    void initContent();
    void updateVolumeLabel();

    // Callbacks
    void onMusicVolumeChanged(cocos2d::CCObject* sender, cocos2d::extension::CCControlEvent event);
    void onEffectsVolumeChanged(cocos2d::CCObject* sender, cocos2d::extension::CCControlEvent event);
    void onCloseClicked(cocos2d::CCObject* sender);

    cocos2d::extension::CCScale9Sprite* m_bgPanel;
    cocos2d::CCNode* m_panelContainer;
    cocos2d::CCLayerColor* m_overlay;
    cocos2d::CCLabelBMFont* m_bgVolumeLabel;
    cocos2d::CCLabelBMFont* m_sfxVolumeLabel;
    cocos2d::extension::CCControlSlider* m_bgVolume;
    cocos2d::extension::CCControlSlider* m_sfxVolume;
};