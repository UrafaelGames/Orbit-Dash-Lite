#pragma once

#include "cocos2d.h"

class CCMenuItemSpriteExtra : public cocos2d::CCMenuItemSprite
{
public:
	static CCMenuItemSpriteExtra* create(cocos2d::CCNode* sprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);

	static CCMenuItemSpriteExtra* create(const char* spriteFrameName, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);

	void setScaleMultiplier(float scale);
	void setDestination(const cocos2d::CCPoint& dest);
	void setSpriteFrame(const char* frameName);
	cocos2d::CCNode* getSprite();
	void setFlippedX(bool flip);

	virtual void selected();
	virtual void unselected();
	virtual void activate();

protected:
	CCMenuItemSpriteExtra();
	bool init();

private:
	cocos2d::CCNode* m_sprite;
	float m_scaleMult;
	float m_selectedScale;
	float m_unselectedScale;
	float m_animDuration;

	cocos2d::CCPoint m_startPos;
	cocos2d::CCPoint m_offsetPos;
	bool m_altAnim;
};