#pragma once

#include "cocos2d.h"
#include <vector>

class PlayerObject;
class OrbitingObstacle;
class ODCenterBox;
class RingNode;
class OBB2D;

class PlayScene : public cocos2d::CCLayer
{
public:
    static cocos2d::CCScene* createScene();
    virtual bool init();

    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

    virtual void update(float dt);

    PlayerObject* m_player;
    std::vector<cocos2d::CCNode*> m_obstacles;
    RingNode* m_ring;
    ODCenterBox* m_centerBox;

private:
    void updateScore(int points);
    void checkCollisions();
    void spawnNewBox();
    void checkAndSpawnObstacles();
    void spawnRandomObstacle();
    void drawOBB(const OBB2D& obb, const cocos2d::ccColor4F& color);
    void showGameOverLayer();
    void pauseGame(cocos2d::CCObject* sender);
    void onDestroyCompleteCallback(CCObject* sender);
    void onBoxDestroyed(int points);
    void restoreRingColor(float dt);

    float m_hitCooldown;
    int m_score;
    cocos2d::CCLabelBMFont* m_scoreLabel;
    bool m_playerHitBoxThisFrame;
    int m_nextObstacleScore;
    int m_obstacleInterval;
};
