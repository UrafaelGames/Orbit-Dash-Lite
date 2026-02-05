#include "ODCenterBox.h"

USING_NS_CC;

ODCenterBox* ODCenterBox::create(const std::string& spriteFile, int maxHits)
{
    ODCenterBox* box = new ODCenterBox();
    if (box && box->init(spriteFile, maxHits))
    {
        box->autorelease();
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;
}

bool ODCenterBox::init(const std::string& spriteFile, int maxHits)
{
    if (!OBBBase::init()) return false;

    m_sprite = CCSprite::create(spriteFile.c_str());
    if (!m_sprite)
    {
        CCLOG("E1 %s", spriteFile.c_str());
        return false;
    }

    m_sprite->setAnchorPoint(ccp(0.5f, 0.5f));
    this->addChild(m_sprite);

    m_hits = 0;
    m_maxHits = maxHits;
    m_points = maxHits * 100;
    m_destroyed = false;

    this->scheduleUpdate();

    return true;
}

void ODCenterBox::update(float dt)
{
    if (getParent())
    {
        float parentRotation = getParent()->getRotation();
        this->setRotation(-parentRotation);
    }

    updateOBB();
}

void ODCenterBox::updateOBB()
{
    CCPoint worldCenter = this->convertToWorldSpace(CCPointZero);

    CCSize spriteSize = m_sprite->getContentSize();
    float scaleX = m_sprite->getScaleX();
    float scaleY = m_sprite->getScaleY();

    CCPoint halfExtents = ccp(
        spriteSize.width * scaleX * 0.5f,
        spriteSize.height * scaleY * 0.5f
    );

    m_obb.set(worldCenter, halfExtents, 0.f);
}

void ODCenterBox::hit()
{
    if (m_destroyed) return;

    m_hits++;
     CCLOG("Collison box! Hits: %d/%d", m_hits, m_maxHits);

    if (m_hits >= m_maxHits)
    {
        m_destroyed = true;
        playDestroyAnimation();

        if (m_onDestroyed)
        {
            m_onDestroyed(m_points);
        }
    }
    else
    {
        playHitAnimation();
    }
}

void ODCenterBox::playHitAnimation()
{
    m_sprite->stopAllActions();

    CCScaleTo* scale1 = CCScaleTo::create(0.05f, 1.2f, 0.9f);
    CCScaleTo* scale2 = CCScaleTo::create(0.05f, 0.9f, 1.2f);
    CCScaleTo* scale3 = CCScaleTo::create(0.05f, 1.0f, 1.0f);

    CCSequence* shake = CCSequence::create(scale1, scale2, scale3, NULL);

    CCTintTo* tint1 = CCTintTo::create(0.05f, 255, 255, 255);
    CCTintTo* tint2 = CCTintTo::create(0.1f, 255, 200, 200);
    CCTintTo* tint3 = CCTintTo::create(0.1f, 255, 255, 255);

    CCSequence* tint = CCSequence::create(tint1, tint2, tint3, NULL);

    m_sprite->runAction(shake);
    m_sprite->runAction(tint);
}

void ODCenterBox::playDestroyAnimation()
{
    m_sprite->stopAllActions();

    CCScaleTo* scaleUp = CCScaleTo::create(0.15f, 1.5f);
    CCFadeOut* fadeOut = CCFadeOut::create(0.15f);

    CCSpawn* spawn = CCSpawn::create(scaleUp, fadeOut, NULL);

    CCCallFuncN* remove = CCCallFuncN::create(this, callfuncN_selector(ODCenterBox::onDestroyComplete));

    m_sprite->runAction(CCSequence::create(spawn, remove, NULL));
}

void ODCenterBox::onDestroyComplete(CCNode* node)
{
    this->removeFromParentAndCleanup(true);
}