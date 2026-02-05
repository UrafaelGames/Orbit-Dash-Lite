#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"

class CCSliderExtra : public cocos2d::extension::CCControlSlider
{
public:
    static CCSliderExtra* create();
    static CCSliderExtra* create(const char* bgFile, const char* progressFile, const char* thumbFile);

    void setBarHeight(float height);
    void setProgressBarHeight(float height);
    void setBarWidth(float width);
    void setProgressBarWidth(float width);
    void setProgressBarWidthOnly(float width);
    void setSliderBallOffset(const cocos2d::CCPoint& offset);
    void setProgressBarOffset(const cocos2d::CCPoint& offset);

    virtual bool initWithSprites(cocos2d::CCSprite* backgroundSprite, cocos2d::CCSprite* progressSprite, cocos2d::CCSprite* thumbSprite);

protected:
    virtual bool init();

    cocos2d::CCPoint m_customBallOffset;
    cocos2d::CCPoint m_customProgressOffset;

    virtual void needsLayout();
};