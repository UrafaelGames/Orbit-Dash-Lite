#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;

class EndGameLayer : public cocos2d::CCLayerColor
{
public:
    static EndGameLayer* create(int score, int bestScore);
    void onEnter();
    void onExit();
    virtual bool init(int score, int bestScore);
    std::string randomLabel();

    void onRetryClicked(cocos2d::CCObject* sender);
    void onHomeClicked(cocos2d::CCObject* sender);

    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void registerWithTouchDispatcher();

private:
    cocos2d::extension::CCScale9Sprite* m_bgPanel;
    cocos2d::CCNode* m_panelContainer;
    cocos2d::CCLayerColor* m_overlay;

    void showLayer();
    void initContent();
    void initPanel();

    int m_score;
    int m_bestScore;
};