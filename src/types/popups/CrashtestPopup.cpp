#include "CrashtestPopup.hpp"

#include "CrashtestShelf.hpp"
#include "Separator.hpp"

using namespace geode::prelude;


CrashtestPopup* CrashtestPopup::create(GeodeVersion defaultVersion) {
	auto ret = new CrashtestPopup;

	if (ret->initAnchored(340.f, 230.f, defaultVersion)) {
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

bool CrashtestPopup::setup(GeodeVersion defaultVersion) {
	this->setTitle("Crashtest");
	auto titleLabel = m_mainLayer->getChildByType<CCLabelBMFont*>(0);
	titleLabel->setScale(0.8f);
	titleLabel->setID("title-label");
	addSideArt(m_buttonMenu, SideArt::Bottom | SideArt::TopRight, SideArtStyle::PopupGold);
	m_currentVersion = defaultVersion;

	auto versionSwitch = createVersionSwitch();
	versionSwitch->setPosition(
		titleLabel->getPositionX() + titleLabel->getScaledContentWidth() / 2.f + 15.f,
		titleLabel->getPositionY() - 2.f
	);
	versionSwitch->setScale(0.5f);
	versionSwitch->setID("version-switch");
	m_mainLayer->addChild(versionSwitch);

	auto directShelf = CrashtestShelf::create(CrashtestShelf::DIRECT, m_currentVersion);
	directShelf->setID("direct-shelf");
	auto scheduledShelf = CrashtestShelf::create(CrashtestShelf::SCHEDULED, m_currentVersion);
	scheduledShelf->setID("scheduled-shelf");

	m_shelvesMenu = CCMenu::create();
	auto shelvesLayout = ColumnLayout::create();
	shelvesLayout->setAxisReverse(true);
	m_shelvesMenu->setLayout(shelvesLayout);

	m_shelvesMenu->addChild(directShelf);
	m_shelvesMenu->addChild(scheduledShelf);
	m_shelvesMenu->setScale(0.9f);
	m_shelvesMenu->setContentHeight(215.f);
	m_shelvesMenu->updateLayout();
	m_shelvesMenu->setID("shelves-menu");
	m_mainLayer->addChildAtPosition(m_shelvesMenu, Anchor::Center, { 0.f, -25.f });

	auto separator = Separator::create({ 255, 255, 255 }, 100.f, 350.f);
	separator->setOpacity(100);
	separator->setID("separator");
	m_mainLayer->addChildAtPosition(separator, Anchor::Center, { 0.f, -15.f });

	this->setID("CrashtestPopup"_spr);
	return true;
}

CCMenu* CrashtestPopup::createVersionSwitch() {
	auto ret = CCMenu::create();
	ret->setAnchorPoint({ 0.f, 0.f });

	auto leftBtn = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png"),
		this,
		menu_selector(CrashtestPopup::onVerLeft)
	);
	leftBtn->setID("version-left-button");
	ret->addChild(leftBtn);

	m_versionLabel = CCLabelBMFont::create(verToString(), "goldFont.fnt");
	standardVerLabelLimit();
	m_versionLabel->setID("version-label");
	ret->addChild(m_versionLabel);

	auto rightBtnSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	rightBtnSpr->setFlipX(true);
	auto rightBtn = CCMenuItemSpriteExtra::create(
		rightBtnSpr,
		this,
		menu_selector(CrashtestPopup::onVerRight)
	);
	rightBtn->setID("version-left-button");
	ret->addChild(rightBtn);

	constexpr float padding = 5.f;
	m_versionLabel->setPositionX(leftBtn->getContentWidth() / 2.f + s_verLabelWidth / 2.f + padding);
	rightBtn->setPositionX(m_versionLabel->getPositionX() + s_verLabelWidth / 2.f + rightBtn->getContentWidth() / 2.f + padding);

	return ret;
}

void CrashtestPopup::onVerCommon() {
	for (auto shelf : CCArrayExt<CrashtestShelf>(m_shelvesMenu->getChildren()))
		shelf->readdCells(m_currentVersion);

	m_versionLabel->setString(verToString());
	standardVerLabelLimit();

	return;
}

char const* CrashtestPopup::verToString() {
	switch (m_currentVersion) {
		case GeodeVersion::V4_6_2:
			return "v4.6.2";

		case GeodeVersion::V4_6_3:
			return "v4.6.3";

		case GeodeVersion::COUNT:
			return "RESERVED_VALUE";

		default:
			return "UNKNOWN";
	}
}

void CrashtestPopup::onVerLeft(CCObject*) {
	if (!static_cast<std::uint8_t>(m_currentVersion)) // if we're at the leftmost version
		return;

	--m_currentVersion;
	
	onVerCommon();

	return;
}

void CrashtestPopup::onVerRight(CCObject*) {
	if (
		static_cast<std::uint8_t>(m_currentVersion) + 1
		==
		static_cast<std::uint8_t>(GeodeVersion::COUNT)
	) // if we're at the rightmost version
		return;

	++m_currentVersion;

	onVerCommon();

	return;
}