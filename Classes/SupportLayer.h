#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"

class SupportLayer : public cocos2d::CCLayerColor
{
public:
    static SupportLayer* create();
    virtual bool init();

    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void registerWithTouchDispatcher();

private:
    cocos2d::extension::CCScale9Sprite* m_bgPanel;
    cocos2d::CCNode* m_panelContainer;
    cocos2d::CCLayerColor* m_overlay;

    void onUrafaelClicked(cocos2d::CCObject* sender);
    void onCocosClicked(cocos2d::CCObject* sender);

    void onEnter();

    void onExit();

    void showLayer();
    void initContent();
    void initPanel();
    void initCloseButton();
    void onCloseClicked(cocos2d::CCObject* sender);
};