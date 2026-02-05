#pragma once
#include "cocos2d.h"

class PopupLayer : public cocos2d::CCLayerColor
{
public:
    static PopupLayer* create();
    void onEnter();
    virtual bool init();

    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

private:
    void showLayer();
    void initPopup();
    void initContent();

    cocos2d::CCSprite* m_bgPanel;
    cocos2d::CCNode* m_panelContainer;
    cocos2d::CCLayerColor* m_overlay;

    void onCloseButtonClicked(cocos2d::CCObject* sender);
    void registerWithTouchDispatcher();
    void onExit();
};