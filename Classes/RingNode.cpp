#include "RingNode.h"

USING_NS_CC;

RingNode* RingNode::create(const char* file, float rotSpeed)
{
	RingNode* ret = new RingNode();
	if (ret && ret->init(file, rotSpeed)) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return NULL;
}

bool RingNode::init(const char* file, float rotSpeed)
{
	if (!CCNode::init()) return false;

	m_sprite = CCSprite::create(file);
	if (!m_sprite) return false;

	m_sprite->setAnchorPoint(ccp(0.5f, 0.5f));
	addChild(m_sprite);

	m_rotationSpeed = rotSpeed;
	m_trackCenterRadius = 0.f;

	if (strcmp(file, "OD_Ring.png") == 0)
		m_trackCenterRadius = 343.f;
	else if (strcmp(file, "OD_Ring.png") == 0)
		m_trackCenterRadius = 312.f;
	else
		m_trackCenterRadius = 300.f;

	scheduleUpdate();
	return true;
}

void RingNode::update(float dt)
{
	if (m_rotationSpeed != 0.f)
		setRotation(getRotation() + m_rotationSpeed * dt);
}

void RingNode::setRotationSpeed(float speed)
{
	m_rotationSpeed = speed;
}

void RingNode::setColorTint(const _ccColor3B& color)
{
	if (m_sprite)
		m_sprite->setColor(color);
}

void RingNode::setOpacityLevel(GLubyte opacity)
{
	if (m_sprite)
		m_sprite->setOpacity(opacity);
}

void RingNode::playImpactAnimation()
{
	if (!m_sprite) return;

	m_sprite->stopAllActions();
	m_sprite->setColor(ccc3(255, 50, 50));

	float shakeAmount = 10.f;
	CCSequence* shake = CCSequence::create(
		CCMoveBy::create(0.05f, ccp(-shakeAmount, shakeAmount)),
		CCMoveBy::create(0.05f, ccp(shakeAmount * 2, -shakeAmount * 2)),
		CCMoveBy::create(0.05f, ccp(-shakeAmount * 2, shakeAmount * 2)),
		CCMoveBy::create(0.05f, ccp(shakeAmount, -shakeAmount)),
		CCMoveBy::create(0.05f, CCPointZero),
		NULL
		);

	CCSequence* scalePulse = CCSequence::create(
		CCScaleTo::create(0.1f, 1.25f, 1.25f),
		CCScaleTo::create(0.2f, 0.95f, 0.95f),
		CCScaleTo::create(0.2f, 1.f, 1.f),
		NULL
		);

	CCDelayTime* delay = CCDelayTime::create(0.3f);
	CCTintTo* tintTo = CCTintTo::create(0.3f, 255, 255, 255);
	CCSequence* colorTransition = CCSequence::create(delay, tintTo, NULL);

	m_sprite->runAction(shake);
	this->runAction(scalePulse);
	m_sprite->runAction(colorTransition);

	CCSequence* flash = CCSequence::create(
		CCFadeTo::create(0.05f, 150),
		CCFadeTo::create(0.05f, 255),
		CCFadeTo::create(0.05f, 150),
		CCFadeTo::create(0.05f, 255),
		NULL
		);
	m_sprite->runAction(flash);
}