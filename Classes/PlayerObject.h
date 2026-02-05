#pragma once
#include "cocos2d.h"
#include "OBB2D.h"
#include "OBBBase.h"
class RingNode;
class PlayerObject : public OBBBase
{
public:
	static PlayerObject* create(const std::string& file);
	virtual bool init(const std::string& file);
	virtual void update(float dt);
	//  void setCenterAndRadius(const cocos2d::CCPoint& center, float radius);
	void attachToRing(RingNode* ring);
	void setAngularSpeed(float degPerSecond);
	OBB2D getOBB() const;
	void onRingTap(std::function<void()> onFinish = nullptr);
	bool isLinearMoving() const;
	void playDeathAnimation(std::function<void()> onComplete = nullptr);
private:
	void onRingTapCallback();
	void onDeathCallback();

	cocos2d::CCSprite* m_sprite;
	cocos2d::CCPoint m_center;
	// cocos2d::CCParticleSystemQuad* m_trailParticles;
	//  cocos2d::PhysicsBody* m_body = nullptr;
	float m_radius;
	float m_angleDeg;
	float m_angularSpeed;
	OBB2D m_obb;
	bool m_isMovingRadially;
	float m_targetRadius;
	float m_baseRadius;
	float m_radialSpeed;
	bool m_isLinearMoving;
	std::function<void()> m_onRingTapFinish;
	std::function<void()> m_onDeathComplete;
};