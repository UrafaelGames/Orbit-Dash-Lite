#include "CCMenuItemSpriteExtra.h"

USING_NS_CC;

CCMenuItemSpriteExtra::CCMenuItemSpriteExtra()
	: m_sprite(NULL)
	, m_scaleMult(1.26f)
	, m_selectedScale(1.f)
	, m_unselectedScale(1.f)
	, m_animDuration(0.3f)
	, m_altAnim(false)
{
}

bool CCMenuItemSpriteExtra::init()
{
	return true;
}

CCMenuItemSpriteExtra* CCMenuItemSpriteExtra::create(CCNode* sprite, CCObject* target, SEL_MenuHandler selector)
{
	CCMenuItemSpriteExtra* ret = new CCMenuItemSpriteExtra();
	if (ret && sprite) {
		ret->m_sprite = sprite;
		ret->initWithNormalSprite(sprite, sprite, sprite, target, selector);

		float scale = sprite->getScale();
		ret->m_unselectedScale = scale;
		ret->m_selectedScale = scale * ret->m_scaleMult;

		sprite->setAnchorPoint(ccp(0.5f, 0.5f));
		sprite->setPosition(ccp(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2));

		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return NULL;
}

CCMenuItemSpriteExtra* CCMenuItemSpriteExtra::create(const char* spriteFrameName, CCObject* target, SEL_MenuHandler selector)
{
	CCSprite* sprite = CCSprite::create(spriteFrameName);
	return create(sprite, target, selector);
}

void CCMenuItemSpriteExtra::setScaleMultiplier(float scale)
{
	m_scaleMult = scale;
	m_selectedScale = m_unselectedScale * scale;
}

void CCMenuItemSpriteExtra::setDestination(const CCPoint& dest)
{
	m_altAnim = true;
	m_startPos = m_sprite->getPosition();
	m_offsetPos = ccpAdd(m_startPos, dest);
}

void CCMenuItemSpriteExtra::selected()
{
	if (m_altAnim) {
		m_sprite->runAction(CCEaseBackInOut::create(CCMoveTo::create(m_animDuration, m_offsetPos)));
	}
	else {
		m_sprite->runAction(CCEaseBounceOut::create(CCScaleTo::create(m_animDuration, m_selectedScale)));
	}

	CCMenuItemSprite::selected();
}

void CCMenuItemSpriteExtra::unselected()
{
	m_sprite->stopAllActions();

	if (m_altAnim) {
		m_sprite->runAction(CCEaseSineInOut::create(CCMoveTo::create(m_animDuration, m_startPos)));
	}
	else {
		m_sprite->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.4f, m_unselectedScale)));
	}

	CCMenuItemSprite::unselected();
}

void CCMenuItemSpriteExtra::activate()
{
	m_sprite->stopAllActions();

	if (m_altAnim)
		m_sprite->setPosition(m_startPos);
	else
		m_sprite->setScale(m_unselectedScale);

	CCMenuItemSprite::activate();
}

CCNode* CCMenuItemSpriteExtra::getSprite()
{
	return m_sprite;
}

void CCMenuItemSpriteExtra::setFlippedX(bool flip)
{
	if (CCSprite* spr = dynamic_cast<CCSprite*>(m_sprite))
		spr->setFlipX(flip);
}