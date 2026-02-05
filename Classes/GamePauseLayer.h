#pragma once

#include "cocos2d.h"

class GamePauseLayer : public cocos2d::CCLayerColor
{
public:
	static GamePauseLayer* create();
	virtual bool init();

private:
	void initPause();
	void initButtons();

	void onResumeClicked(cocos2d::CCObject* sender);
	void onRetryClicked(cocos2d::CCObject* sender);
	void onSettingsClicked(cocos2d::CCObject* sender);
	void onExitClicked(cocos2d::CCObject* sender);
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void registerWithTouchDispatcher() override;
	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

	cocos2d::CCNode* m_panelContainer;
	cocos2d::CCLayerColor* m_overlay;
};