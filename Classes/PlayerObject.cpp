#include "PlayerObject.h"
#include "RingNode.h"
#include <algorithm>

USING_NS_CC;

PlayerObject* PlayerObject::create(const std::string& file)
{
	PlayerObject* ret = new (std::nothrow) PlayerObject();
	if (ret && ret->init(file))
	{
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

bool PlayerObject::init(const std::string& file)
{
	if (!CCNode::init()) return false;

	m_sprite = CCSprite::create(file.c_str());
	if (!m_sprite) return false;

	m_sprite->setAnchorPoint(ccp(0.5f, 0.5f));
	addChild(m_sprite);

	m_center = CCPointZero;
	m_radius = 100.f;
	m_angleDeg = 0.f;
	m_angularSpeed = 120.f;
	m_isLinearMoving = false;

	CCSize spriteSize = m_sprite->getContentSize();
	float scaleX = m_sprite->getScaleX();
	float scaleY = m_sprite->getScaleY();

	float reductionFactor = 0.7f;
	CCPoint halfExtents = ccp(
		spriteSize.width * scaleX * 0.5f * reductionFactor,
		spriteSize.height * scaleY * 0.5f * reductionFactor
		);


	m_obb.set(getPosition(), halfExtents, 0.f);
	scheduleUpdate();

	return true;
}

/*void PlayerObject::setCenterAndRadius(const CCPoint& center, float radius)
{
m_center = center;
m_radius = radius;
}  */

float getRingTrackCenterRadius(const std::string& filename)
{
	CCImage* image = new CCImage();
	if (!image->initWithImageFile(filename.c_str()))
	{
		CCLOG("E2: %s", filename.c_str());
		delete image;
		return 0.f;
	}

	int width = image->getWidth();
	int height = image->getHeight();
	unsigned char* data = image->getData();
	float cx = width / 2.f;
	float cy = height / 2.f;

	std::vector<float> trackDistances;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x) {
			int i = (y * width + x) * 4;
			unsigned char r = data[i + 0];
			unsigned char g = data[i + 1];
			unsigned char b = data[i + 2];
			if (r > 220 && g > 220 && b > 220)
			{
				float dx = x - cx;
				float dy = y - cy;
				float dist = sqrtf(dx * dx + dy * dy);
				trackDistances.push_back(dist);
			}
		}
	}

	delete image;

	if (trackDistances.empty()) return 0.f;

	std::sort(trackDistances.begin(), trackDistances.end());
	size_t mid = trackDistances.size() / 2;
	return trackDistances[mid];
}

void PlayerObject::attachToRing(RingNode* ring)
{
	CCSprite* ringSprite = ring->getSprite();
	float ringOuterRadius = ringSprite->getContentSize().width * ringSprite->getScaleX() * 0.5f;

	// float whiteCenterRadius = getRingTrackCenterRadius("OD_Ring.png") * ring->getScaleX();

	float whiteCenterRadius = ringOuterRadius * 1.01f;

	float playerRadius = m_sprite->getContentSize().width * m_sprite->getScaleX() * 0.5f;
	m_radius = whiteCenterRadius - playerRadius * 0.5f;

	m_center = ring->getPosition();
	m_angleDeg = 0.f;

	bool showDebug = false;

	CCLOG("Ring -> Outer: %.1f | Mid: %.1f", ringOuterRadius, m_radius);

	if (showDebug)
	{
		CCDrawNode* dbg = CCDrawNode::create();
		dbg->drawDot(m_center, ringOuterRadius, ccc4f(0, 1, 0, 0.3f));
		dbg->drawDot(m_center, m_radius, ccc4f(1, 0, 0, 0.3f));
		ring->getParent()->addChild(dbg);
	}

	m_baseRadius = m_radius;
	m_targetRadius = m_radius;
	m_isMovingRadially = false;
	m_radialSpeed = 600.f;
}


void PlayerObject::setAngularSpeed(float degPerSecond)
{
	m_angularSpeed = degPerSecond;
}

void PlayerObject::update(float dt)
{
	if (!m_isLinearMoving)
	{
		m_angleDeg -= m_angularSpeed * dt;
		float rad = CC_DEGREES_TO_RADIANS(m_angleDeg);

		CCPoint pos;
		pos.x = m_center.x + cosf(rad) * m_radius;
		pos.y = m_center.y + sinf(rad) * m_radius;

		setPosition(pos);
		setRotation(-m_angleDeg);
	}

	CCSize spriteSize = m_sprite->getContentSize();
	float scaleX = m_sprite->getScaleX();
	float scaleY = m_sprite->getScaleY();

	float reductionFactor = 0.7f;
	CCPoint halfExtents = ccp(
		spriteSize.width * scaleX * 0.5f * reductionFactor,
		spriteSize.height * scaleY * 0.5f * reductionFactor
		);

	m_obb.set(getPosition(), halfExtents, getRotation());
}


OBB2D PlayerObject::getOBB() const
{
	return m_obb;
}

void PlayerObject::onRingTap(std::function<void()> onFinish)
{
	m_isLinearMoving = true;

	CCPoint p = getPosition();
	CCPoint c = m_center;
	CCPoint target = ccp(c.x * 2 - p.x, c.y * 2 - p.y);

	float duration = 0.25f;

	stopAllActions();

	runAction(CCSequence::create(
		CCMoveTo::create(duration, target),
		CCCallFunc::create(this, callfunc_selector(PlayerObject::onRingTapCallback)),
		NULL
		));

	m_onRingTapFinish = onFinish;
}

void PlayerObject::onRingTapCallback()
{
	CCPoint dir = ccpSub(getPosition(), m_center);
	m_angleDeg = CC_RADIANS_TO_DEGREES(atan2f(dir.y, dir.x));

	m_isLinearMoving = false;

	if (m_onRingTapFinish)
		m_onRingTapFinish();
}

bool PlayerObject::isLinearMoving() const
{
	return m_isLinearMoving;
}

void PlayerObject::playDeathAnimation(std::function<void()> onComplete)
{
	m_isLinearMoving = true;
	stopAllActions();
	unscheduleUpdate();

	CCSpawn* explosion = CCSpawn::create(
		CCRotateBy::create(0.3f, 720.f),

		CCSequence::create(
		CCScaleTo::create(0.1f, 1.5f),
		CCScaleTo::create(0.2f, 0.f),
		NULL
		),
		CCFadeOut::create(0.3f),
		NULL
		);

	m_sprite->runAction(CCSequence::create(
		CCTintTo::create(0.05f, 255, 0, 0),
		NULL
		));

	m_onDeathComplete = onComplete;

	this->runAction(CCSequence::create(
		explosion,
		CCCallFunc::create(this, callfunc_selector(PlayerObject::onDeathCallback)),
		NULL
		));
}

void PlayerObject::onDeathCallback()
{
	if (m_onDeathComplete)
		m_onDeathComplete();

	this->removeFromParent();
}