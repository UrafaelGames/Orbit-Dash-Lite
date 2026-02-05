#pragma once

#include "cocos2d.h"

class LoadingScene : public cocos2d::CCLayer
{
public:
	static cocos2d::CCScene* createScene();
	virtual bool init();

	std::string getLoadSplash();

private:
	void onLogoAnimationComplete();
};