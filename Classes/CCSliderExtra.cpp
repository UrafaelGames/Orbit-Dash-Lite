#include "CCSliderExtra.h"

USING_NS_CC;

CCSliderExtra* CCSliderExtra::create()
{
    CCSliderExtra* slider = new CCSliderExtra();
    if (slider && slider->init())
    {
        slider->autorelease();
        return slider;
    }
    CC_SAFE_DELETE(slider);
    return NULL;
}

CCSliderExtra* CCSliderExtra::create(const char* bgFile, const char* progressFile, const char* thumbFile)
{
    CCSprite* backgroundSprite = CCSprite::create(bgFile);
    CCSprite* progressSprite = CCSprite::create(progressFile);
    CCSprite* thumbSprite = CCSprite::create(thumbFile);

    CCSliderExtra* slider = new CCSliderExtra();
    if (slider && slider->initWithSprites(backgroundSprite, progressSprite, thumbSprite))
    {
        slider->autorelease();
        return slider;
    }
    CC_SAFE_DELETE(slider);
    return NULL;
}

bool CCSliderExtra::init()
{
    if (!CCControlSlider::init())
        return false;

    m_customBallOffset = CCPointZero;
    m_customProgressOffset = CCPointZero;

    return true;
}

bool CCSliderExtra::initWithSprites(CCSprite* backgroundSprite, CCSprite* progressSprite, CCSprite* thumbSprite)
{
    if (!CCControlSlider::initWithSprites(backgroundSprite, progressSprite, thumbSprite))
        return false;

    m_customBallOffset = CCPointZero;
    m_customProgressOffset = CCPointZero;

    return true;
}

void CCSliderExtra::setBarHeight(float height)
{
    CCSprite* bg = getBackgroundSprite();
    if (bg)
    {
        CCSize size = bg->getContentSize();
        bg->setScaleY(height / size.height);
    }
}

void CCSliderExtra::setProgressBarHeight(float height)
{
    CCSprite* progress = getProgressSprite();
    if (progress)
    {
        CCSize size = progress->getContentSize();
        progress->setScaleY(height / size.height);
    }
}

void CCSliderExtra::setBarWidth(float width)
{
    CCSprite* bg = getBackgroundSprite();
    if (bg)
    {
        CCSize size = bg->getContentSize();
        bg->setScaleX(width / size.width);
    }
}

void CCSliderExtra::setProgressBarWidth(float width)
{
    CCSprite* progress = getProgressSprite();
    if (progress)
    {
        CCSize size = progress->getContentSize();
        progress->setScaleX(width / size.width);
    }
}

void CCSliderExtra::setProgressBarWidthOnly(float width)
{
    CCSprite* progress = getProgressSprite();
    if (progress)
    {
        CCSize originalSize = progress->getContentSize();
        progress->setScaleX(width / originalSize.width);
    }
}

void CCSliderExtra::setSliderBallOffset(const CCPoint& offset)
{
    m_customBallOffset = offset;
    needsLayout();
}

void CCSliderExtra::setProgressBarOffset(const CCPoint& offset)
{
    m_customProgressOffset = offset;
    needsLayout();
}

void CCSliderExtra::needsLayout()
{
    CCControlSlider::needsLayout();

    CCSprite* thumb = getThumbSprite();
    if (thumb)
    {
        CCPoint currentPos = thumb->getPosition();
        thumb->setPosition(
            ccp(currentPos.x + m_customBallOffset.x,
                currentPos.y + m_customBallOffset.y)
        );
    }

    CCSprite* progress = getProgressSprite();
    if (progress)
    {
        CCPoint currentPos = progress->getPosition();
        progress->setPosition(
            ccp(currentPos.x + m_customProgressOffset.x,
                currentPos.y + m_customProgressOffset.y)
        );
    }
}