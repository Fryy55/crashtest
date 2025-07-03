#pragma once

#include <Geode/Geode.hpp>

#include "enums.hpp"
#include "CrashtestShelf.hpp"


class CrashtestCell final : public cocos2d::CCNodeRGBA {
public:
	static CrashtestCell* create(CrashType crashType, GeodeVersion version, CrashtestShelf::Type shelfType);

	void setOpacity(GLubyte opacity) override;

private:
	bool init(CrashType, GeodeVersion, CrashtestShelf::Type);

	char const* stringForCrashType(CrashType);
	char const* crashlogInfoSprite(CrashType, GeodeVersion);

	void onCrash();
	void onCrashForward(float) { this->onCrash(); }

	// Fields
	cocos2d::extension::CCScale9Sprite* m_bg;
	cocos2d::CCMenu* m_mainMenu;
	CCMenuItemSpriteExtra* m_crashBtn;
	GLubyte m_bgOpacity;
	CrashtestShelf::Type m_shelfType;

	static constexpr cocos2d::CCSize s_contentSize { 80.f, 100.f };
};