#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

class ButtonSprite : public cocos2d::CCSprite
{
private:
	cocos2d::extension::CCScale9Sprite* m_buttonTexture;
	cocos2d::CCSprite* m_subSprite;
	cocos2d::CCSprite* m_subBGSprite;

	float m_minWidth;
	cocos2d::CCPoint m_spriteOffset;

	cocos2d::CCLabelBMFont* m_label;

	float m_scale;
	float m_width;
	float m_height;

	bool m_absoluteWidth;
	bool m_hasTextProvided;
	bool m_isEnabled;

public:
	static ButtonSprite* create(const std::string& caption, int with, int minWidth, float scale, bool absoluteWidth, const std::string& font, const std::string& texture, float height);

	static ButtonSprite* create(cocos2d::CCSprite* iconSprite, int width, int minWidth, float height, float scale, bool absoluteWidth, const std::string& texture, bool dontUseScale9);

	static ButtonSprite* create(const std::string& caption, int width, float minWidth, float scale);
	static ButtonSprite* create(const std::string& caption);

	bool initWithText(const std::string& caption, int width, int minWidth, float scale, bool absoluteWidth, const std::string& font, const std::string& texture, float height);
	bool initWithSprite(cocos2d::CCSprite* iconSprite, int width, int minWidth, float height, float scale, bool absoluteWidth, const std::string& texture, bool dontUseScale9);

	void updateBGImage(const std::string& file);
	void setString(const std::string& str);
	void setColor(const cocos2d::ccColor3B& color);
	void updateSpriteBGSize();

	void setEnabled(bool enabled);
	bool isEnabled() const { return m_isEnabled; }
};