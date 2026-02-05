#pragma once

#include "cocos2d.h"

class RingNode : public cocos2d::CCNode
{
private:
	cocos2d::CCSprite* m_sprite;
	float m_rotationSpeed;
	float m_trackCenterRadius;

public:
	static RingNode* create(const char* file, float rotSpeed = 0.f);

	virtual bool init(const char* file, float rotSpeed);
	virtual void update(float dt);


	void setRotationSpeed(float speed);
	void setColorTint(const cocos2d::_ccColor3B& color);
	void setOpacityLevel(GLubyte opacity);
	float getTrackCenterRadius() const { return m_trackCenterRadius; }

	cocos2d::CCSprite* getSprite() const { return m_sprite; }

	void playImpactAnimation();
};