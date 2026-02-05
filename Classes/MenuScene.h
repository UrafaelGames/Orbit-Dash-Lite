#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"

class MenuScene : public cocos2d::CCLayer
{
public:
    static cocos2d::CCScene* createScene();
    virtual bool init();

private:
    void onPlayClicked(CCObject* sender);
    void onCustomizeClicked(CCObject* sender);
    void onLeaderboardClicked(CCObject* sender);
    void onAboutClicked(CCObject* sender);
    void onSettingsClicked(CCObject* sender);
    void onMusicClicked(CCObject* sender);
};

#endif