#pragma once

#include "cocos2d.h"
#include "OBB2D.h"
#include "OBBBase.h"
#include <functional>

class ODCenterBox : public OBBBase
{
public:
    static ODCenterBox* create(const std::string& spriteFile, int maxHits = 3);
    bool init(const std::string& spriteFile, int maxHits);

    virtual void update(float dt);

    virtual OBB2D getOBB() const { return m_obb; }

    void hit();
    bool isDestroyed() const { return m_hits >= m_maxHits; }

    int getHits() const { return m_hits; }
    int getMaxHits() const { return m_maxHits; }
    int getPoints() const { return m_points; }

    void setOnDestroyCallback(std::function<void(int points)> callback)
    {
        m_onDestroyed = callback;
    }

    void onDestroyComplete(cocos2d::CCNode* node);

private:
    void updateOBB();
    void playHitAnimation();
    void playDestroyAnimation();

    cocos2d::CCSprite* m_sprite;
    OBB2D m_obb;

    int m_hits;
    int m_maxHits;
    int m_points;
    bool m_destroyed;

    std::function<void(int points)> m_onDestroyed;
};
