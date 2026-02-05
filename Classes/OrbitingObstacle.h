#pragma once

#include "cocos2d.h"
#include "OBBBase.h"
#include "OBB2D.h"

class OrbitingObstacle : public OBBBase  
{
public:
    static OrbitingObstacle* create(
        const std::string& spriteFile,
        float radius,
        float angularSpeedDeg,
        float startAngleDeg,
        float additionalRotation = 0.f
    );

    bool init(
        const std::string& spriteFile,
        float radius,
        float angularSpeedDeg,
        float startAngleDeg,
        float additionalRotation = 0.f
    );

    virtual void update(float dt);
    virtual OBB2D getOBB() const { return m_obb; }
    cocos2d::CCSprite* getSprite() const { return m_sprite; }

    void setShowDebug(bool show);
    void updateDebugDraw();

private:
    void updateOBB();

    cocos2d::CCSprite* m_sprite;
    float m_radius;
    float m_angularSpeed;
    float m_angleDeg;
    float m_additionalRotation;
    OBB2D m_obb;

    cocos2d::CCDrawNode* m_debugDrawNode;
    bool m_showDebug;
};
