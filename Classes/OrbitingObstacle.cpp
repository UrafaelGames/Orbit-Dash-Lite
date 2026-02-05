#include "OrbitingObstacle.h"

USING_NS_CC;

OrbitingObstacle* OrbitingObstacle::create(
    const std::string& spriteFile,
    float radius,
    float angularSpeedDeg,
    float startAngleDeg,
    float additionalRotation)
{
    OrbitingObstacle* o = new OrbitingObstacle();
    if (o && o->init(spriteFile, radius, angularSpeedDeg, startAngleDeg, additionalRotation))
    {
        o->autorelease();
        return o;
    }
    CC_SAFE_DELETE(o);
    return NULL;
}

bool OrbitingObstacle::init(
    const std::string& spriteFile,
    float radius,
    float angularSpeedDeg,
    float startAngleDeg,
    float additionalRotation)
{
    if (!OBBBase::init()) return false; 

    m_sprite = CCSprite::create(spriteFile.c_str());
    if (!m_sprite)
    {
        CCLOG("E1: %s", spriteFile.c_str());
        return false;
    }

    m_sprite->setAnchorPoint(ccp(0.5f, 0.5f));
    this->addChild(m_sprite);

    m_radius = radius;
    m_angularSpeed = angularSpeedDeg;
    m_angleDeg = startAngleDeg;
    m_additionalRotation = additionalRotation;

    m_debugDrawNode = NULL;
    m_showDebug = false;

    float rad = CC_DEGREES_TO_RADIANS(m_angleDeg);
    CCPoint centerPos;
    centerPos.x = cosf(rad) * m_radius;
    centerPos.y = sinf(rad) * m_radius;

    this->setPosition(centerPos);
    this->setRotation(-m_angleDeg + m_additionalRotation); 

    this->scheduleUpdate(); 

    return true;
}

void OrbitingObstacle::update(float dt)
{
    m_angleDeg += m_angularSpeed * dt;

    while (m_angleDeg >= 360.0f) m_angleDeg -= 360.0f;
    while (m_angleDeg < 0.0f) m_angleDeg += 360.0f;

    float rad = CC_DEGREES_TO_RADIANS(m_angleDeg);

    CCPoint centerPos;
    centerPos.x = cosf(rad) * m_radius;
    centerPos.y = sinf(rad) * m_radius;

    this->setPosition(centerPos);  
    this->setRotation(-m_angleDeg + m_additionalRotation); 

    updateOBB();

    if (m_showDebug)
    {
        updateDebugDraw();
    }
}

void OrbitingObstacle::updateOBB()
{
    CCPoint worldCenter = this->convertToWorldSpace(CCPointZero);

    CCSize spriteSize = m_sprite->getContentSize();
    float scaleX = m_sprite->getScaleX();
    float scaleY = m_sprite->getScaleY();

    float reductionFactor = 0.65f;
    CCPoint halfExtents = ccp(
        spriteSize.width * scaleX * 0.5f * reductionFactor,
        spriteSize.height * scaleY * 0.5f * reductionFactor
    );

    float orbitalRotation = -m_angleDeg;

    m_obb.set(worldCenter, halfExtents, orbitalRotation);
}

void OrbitingObstacle::updateDebugDraw()
{

    if (m_showDebug) {
        CCPoint corners[4];
        m_obb.getCorners(corners);
    }
}

void OrbitingObstacle::setShowDebug(bool show)
{
    m_showDebug = show;

    if (!show && m_debugDrawNode) {
        m_debugDrawNode->removeFromParentAndCleanup(true);
        m_debugDrawNode = NULL;
    }
}