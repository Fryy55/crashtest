#include "CrashtestShelf.hpp"

#include "CrashtestCell.hpp"

using namespace geode::prelude;


CrashtestShelf* CrashtestShelf::create(CrashtestShelf::Type type, GeodeVersion defaultVersion) {
	auto ret = new CrashtestShelf;

	if (ret->init(type, defaultVersion)) {
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

void CrashtestShelf::addCells(GeodeVersion version) {
	for (std::size_t cellIndex = 0u; cellIndex < static_cast<std::size_t>(CrashType::COUNT); ++cellIndex) {
		auto cell = CrashtestCell::create(static_cast<CrashType>(cellIndex), version, m_shelfType);
		cell->runAction(
			CCSequence::createWithTwoActions(
				CCDelayTime::create(0.1f * cellIndex),
				CCFadeIn::create(0.3f)
			)
		);
		cell->setID(
			fmt::format(
				"{}-cell-{}",
				m_shelfType ? "scheduled" : "direct",
				cellIndex
			)
		);
		m_menu->addChild(cell);
	}

	m_menu->updateLayout();
	return;
}

void CrashtestShelf::readdCells(GeodeVersion newVersion) {
	constexpr float fadeDuration = 0.25f;
	for (auto cell : CCArrayExt<CrashtestCell>(m_menu->getChildren())) {
		cell->stopAllActions();
		cell->runAction(CCSequence::createWithTwoActions(
			CCFadeTo::create(fadeDuration, 0),
			CallFuncExt::create([cell]() { cell->removeFromParent(); })
		));
	}

	this->stopAllActions();
	this->runAction(CCSequence::createWithTwoActions(
		CCDelayTime::create(fadeDuration),
		CallFuncExt::create([this, newVersion]() { this->addCells(newVersion); })
	));

	return;
}

bool CrashtestShelf::init(CrashtestShelf::Type type, GeodeVersion defaultVersion) {
	if (!CCMenu::init())
		return false;
	this->setContentHeight(160.f);
	m_shelfType = type;

	if (m_shelfType == DIRECT)
		m_label = CCLabelBMFont::create("Direct Crashes", "bigFont.fnt");
	else
		m_label = CCLabelBMFont::create("Scheduled Crashes", "bigFont.fnt");
	m_label->setScale(0.7f);
	m_label->setID("label");
	m_menu = CCMenu::create();
	m_menu->setLayout(RowLayout::create());
	m_menu->setID("menu");

	this->addCells(defaultVersion);

	this->addChildAtPosition(m_label, Anchor::Top);
	this->addChildAtPosition(m_menu, Anchor::Center);

	return true;
}