#include <Geode/Geode.hpp>

#include "CrashtestPopup.hpp"

#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;


class $modify(MenuLayer) {
	$override bool init() {
		if (!MenuLayer::init())
			return false;

		auto rightMenu = this->getChildByID("right-side-menu");
		auto crashtestBtn = CCMenuItemExt::createSpriteExtraWithFrameName(
			"GJ_reportBtn_001.png",
			0.75f,
			[](CCMenuItemSpriteExtra*) {
				CrashtestPopup::create(GeodeVersion::V4_6_3)->show(); // change default version if anything
			}
		);
		crashtestBtn->setID("crashtest-button"_spr);
		rightMenu->addChild(crashtestBtn);
		rightMenu->updateLayout();

		return true;
	}
};