#pragma once

#include <Geode/Geode.hpp>

#include "enums.hpp"

class CrashtestPopup final : public geode::Popup<GeodeVersion> {
public:
	static CrashtestPopup* create(GeodeVersion defaultVersion);

private:
	bool setup(GeodeVersion) override;

	cocos2d::CCMenu* createVersionSwitch();
	void onVerCommon();

	char const* verToString();
	void standardVerLabelLimit() { m_versionLabel->limitLabelWidth(s_verLabelWidth, 1.f, 0.1f); }

	void onVerLeft(cocos2d::CCObject*);
	void onVerRight(cocos2d::CCObject*);

	// Fields
	cocos2d::CCLabelBMFont* m_versionLabel;
	GeodeVersion m_currentVersion;
	cocos2d::CCMenu* m_shelvesMenu;

	static constexpr float s_verLabelWidth = 70.f;
};