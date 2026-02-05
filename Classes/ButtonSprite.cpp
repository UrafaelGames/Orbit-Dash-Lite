#include "ButtonSprite.h"

USING_NS_CC;

ButtonSprite* ButtonSprite::create(const std::string& caption, int width, int minWidth, float scale, bool absoluteWidth, const std::string& font, const std::string& texture, float height)
{
	ButtonSprite* ret = new ButtonSprite();
	if (ret && ret->initWithText(caption, width, minWidth, scale, absoluteWidth, font, texture, height))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return NULL;
}

ButtonSprite* ButtonSprite::create(CCSprite* iconSprite, int width, int minWidth, float height, float scale, bool absoluteWidth, const std::string& texture, bool dontUseScale9)
{
	ButtonSprite* ret = new ButtonSprite();
	if (ret && ret->initWithSprite(iconSprite, width, minWidth, height, scale, absoluteWidth, texture, dontUseScale9))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return NULL;
}

ButtonSprite* ButtonSprite::create(const std::string& caption, int width, float minWidth, float scale)
{
	return ButtonSprite::create(caption, width, (int)minWidth, scale, false, "fonts/BigFont-Gold.fnt", "OD_Button_01.png", 0);
}

ButtonSprite* ButtonSprite::create(const std::string& caption)
{
	return ButtonSprite::create(caption, 0, 0, 1.0f);
}

bool ButtonSprite::initWithText(const std::string& caption, int width, int minWidth, float scale, bool absoluteWidth, const std::string& font, const std::string& texture, float height)
{
	if (!CCSprite::create()) return false;

	m_isEnabled = true;
	m_hasTextProvided = true;
	m_scale = scale;
	m_width = (float)width;
	m_height = height;
	m_absoluteWidth = absoluteWidth;
	m_minWidth = (float)minWidth;
	m_spriteOffset = ccp(0, 0);

	m_label = CCLabelBMFont::create("", font.c_str());
	this->addChild(m_label, 1);

	m_buttonTexture = cocos2d::extension::CCScale9Sprite::create(texture.c_str());
	m_buttonTexture->setCapInsets(CCRect(12, 12, m_buttonTexture->getContentSize().width - 24, m_buttonTexture->getContentSize().height - 24));
	this->addChild(m_buttonTexture, 0);

	setString(caption);
	return true;
}

bool ButtonSprite::initWithSprite(CCSprite* iconSprite, int width, int minWidth, float height, float scale, bool absoluteWidth, const std::string& texture, bool dontUseScale9)
{
	if (!CCSprite::init())
		return false;

	m_isEnabled = true;
	m_hasTextProvided = false;
	m_subSprite = iconSprite;
	m_scale = scale;
	m_width = (float)width;
	m_absoluteWidth = absoluteWidth;
	m_minWidth = (float)minWidth;
	m_spriteOffset = ccp(0, 0);

	this->addChild(m_subSprite, 1);

	if (dontUseScale9)
	{
		m_subBGSprite = CCSprite::create(texture.c_str());
		this->addChild(m_subBGSprite, 0);
	}
	else
	{
		m_buttonTexture = cocos2d::extension::CCScale9Sprite::create(texture.c_str(), CCRect(0, 0, 40, 40));
		m_buttonTexture->setContentSize(CCSize(16, 16));
		this->addChild(m_buttonTexture, 0);
	}

	updateSpriteBGSize();
	return true;
}


void ButtonSprite::updateBGImage(const std::string& file)
{
	if (m_buttonTexture)
	{
		CCSize oldSize = m_buttonTexture->getContentSize();
		m_buttonTexture->removeFromParent();

		m_buttonTexture = cocos2d::extension::CCScale9Sprite::create(
			file.c_str(),
			CCRect(0, 0, 40, 40)
			);
		m_buttonTexture->setContentSize(oldSize);
		this->addChild(m_buttonTexture, 0);
	}
	else if (m_subBGSprite)
	{
		m_subBGSprite->setTexture(
			CCTextureCache::sharedTextureCache()->addImage(file.c_str())
			);
	}
}

void ButtonSprite::setString(const std::string& str)
{
	if (!m_hasTextProvided)
		return;

	m_label->setScale(1.0f);
	m_label->setString(str.c_str());

	if (m_width > 0 && m_label->getContentSize().width > m_width)
	{
		m_label->setScale(m_width / m_label->getContentSize().width);
	}

	if (m_label->getScale() > m_scale)
		m_label->setScale(m_scale);

	float hPadding = 50.f;
	float vPadding = 18.f;

	float finalWidth = m_absoluteWidth
		? m_width
		: MAX(m_minWidth, m_label->getContentSize().width * m_label->getScale() + hPadding * 2);

	float finalHeight = (m_height > 0)
		? m_height
		: m_label->getContentSize().height + vPadding * 2;

	CCSize size(finalWidth, finalHeight);

	m_buttonTexture->setContentSize(size);
	this->setContentSize(size);

	m_label->setPosition(ccp(size.width / 2, size.height / 2));
	m_buttonTexture->setPosition(ccp(size.width / 2, size.height / 2));
}

void ButtonSprite::setColor(const ccColor3B& color)
{
	if (m_label) m_label->setColor(color);
	if (m_buttonTexture) m_buttonTexture->setColor(color);
	if (m_subSprite) m_subSprite->setColor(color);
	if (m_subBGSprite) m_subBGSprite->setColor(color);
}

void ButtonSprite::updateSpriteBGSize()
{
	if (!m_subSprite)
		return;

	float width = m_subSprite->getContentSize().width * m_subSprite->getScale();
	float height = m_subSprite->getContentSize().height * m_subSprite->getScale();

	CCSize size(
		MAX(m_minWidth, width + 16),
		height + 12
		);

	this->setContentSize(size);

	m_subSprite->setPosition(ccp(size.width / 2, size.height / 2));

	if (m_buttonTexture)
		m_buttonTexture->setContentSize(size),
		m_buttonTexture->setPosition(ccp(size.width / 2, size.height / 2));

	if (m_subBGSprite)
		m_subBGSprite->setPosition(ccp(size.width / 2, size.height / 2));
}

void ButtonSprite::setEnabled(bool enabled)
{
	m_isEnabled = enabled;

	ccColor3B color = enabled ? ccc3(255, 255, 255) : ccc3(120, 120, 120);
	GLubyte opacity = enabled ? 255 : 150;

	if (m_label)
		m_label->setColor(color), m_label->setOpacity(opacity);

	if (m_buttonTexture)
		m_buttonTexture->setColor(color), m_buttonTexture->setOpacity(opacity);

	if (m_subSprite)
		m_subSprite->setColor(color), m_subSprite->setOpacity(opacity);

	if (m_subBGSprite)
		m_subBGSprite->setColor(color), m_subBGSprite->setOpacity(opacity);
}