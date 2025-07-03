#include "CrashtestCell.hpp"

#include <Geode/utils/terminate.hpp>
#include "constants.hpp"

using namespace geode::prelude;


CrashtestCell* CrashtestCell::create(CrashType crashType, GeodeVersion version, CrashtestShelf::Type shelfType) {
	auto ret = new CrashtestCell;

	if (ret->init(crashType, version, shelfType)) {
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

void CrashtestCell::setOpacity(GLubyte opacity) {
	this->CCNodeRGBA::setOpacity(opacity);
	m_mainMenu->setOpacity(opacity);
	m_bg->setOpacity(opacity * m_bgOpacity / 255);

	return;
}

bool CrashtestCell::init(CrashType crashType, GeodeVersion version, CrashtestShelf::Type shelfType) {
	if (!CCNodeRGBA::init())
		return false;
	this->setContentSize(s_contentSize);
	m_shelfType = shelfType;

	m_bg = CCScale9Sprite::create("square02_small.png");
	m_bg->setOpacity(m_bgOpacity = m_shelfType ? 100 : 50);
	m_bg->setContentSize(s_contentSize);
	m_bg->setID("background");
	this->addChildAtPosition(m_bg, Anchor::Center);

	m_mainMenu = CCMenu::create();
	m_mainMenu->setContentSize(s_contentSize);
	m_mainMenu->setID("main-menu");
	this->addChildAtPosition(m_mainMenu, Anchor::Center);

	auto label = CCLabelBMFont::create(stringForCrashType(crashType), "bigFont.fnt");
	label->limitLabelWidth(75.f, 1.f, 0.1f);
	label->setID("crash-description-label");
	m_mainMenu->addChildAtPosition(label, Anchor::Top, { 0.f, -10.f });

	m_crashBtn = CCMenuItemExt::createSpriteExtraWithFrameName(
		"GJ_deleteSongBtn_001.png",
		1.5f,
		m_shelfType ?
			std::function<void(CCMenuItemSpriteExtra*)> {
				[this](CCMenuItemSpriteExtra*) {
					this->scheduleOnce(schedule_selector(CrashtestCell::onCrashForward), 0.f);
				}
			}
			:
			std::function<void(CCMenuItemSpriteExtra*)>{
				[this](...) {
					this->onCrash();
				}
			}
	);
	m_crashBtn->setTag(static_cast<int>(crashType));
	m_crashBtn->setID("crash-button");
	m_mainMenu->addChildAtPosition(m_crashBtn, Anchor::Center);

	auto crashlogLabel = CCLabelBMFont::create("Crashlog", "bigFont.fnt");
	crashlogLabel->setAnchorPoint({ 0.f, 0.f });
	crashlogLabel->limitLabelWidth(60.f, 1.f, 0.1f);
	crashlogLabel->setID("crashlog-label");
	m_mainMenu->addChildAtPosition(crashlogLabel, Anchor::BottomLeft, { 3.f, 5.f });

	auto crashlogInfoBtn = CCMenuItemExt::createSpriteExtraWithFrameName(
		crashlogInfoSprite(crashType, version),
		0.75f,
		[](CCMenuItemSpriteExtra*) {
			FLAlertLayer::create(
				"Crashlog tests",
				"This mark shows what my own tests have led to.\n <cr>X</c> means I got no crashlog, "
				"<cg>V</c> means I got a crashlog, <cc>?</c> means I haven't tested this crash on "
				"this version yet, and <ca>!</c> - ...you should never see this one.",
				"OK"
			)->show();
		}
	);
	crashlogInfoBtn->setID("crashlog-info-button");
	m_mainMenu->addChildAtPosition(
		crashlogInfoBtn,
		Anchor::BottomRight,
		{ -crashlogInfoBtn->getContentWidth() / 2.f - 3.f, crashlogInfoBtn->getContentHeight() / 2.f + 4.f }
	);

	this->setOpacity(0);

	return true;
}

char const* CrashtestCell::stringForCrashType(CrashType crashType) {
	switch (crashType) {
		case CrashType::NULLPTR_DEREF:
			return "Undefined Behavior";

		case CrashType::UTILS_TERMINATE:
			return "utils::terminate";

		case CrashType::THROW:
			return "throw Operator";

		case CrashType::STD_EXCEPT:
			return "STL Exception";

		case CrashType::STD_ABORT:
			return "std::abort";

		case CrashType::COUNT:
			return "RESERVED_VALUE";

		default:
			return "UNKNOWN";
	}
}

char const* CrashtestCell::crashlogInfoSprite(CrashType crashType, GeodeVersion version) {
	std::optional<bool> value;
	try {
		value = m_shelfType ?
			constants::crashData
				.at(static_cast<std::size_t>(version))
				.second
				.at(static_cast<std::size_t>(crashType))
			:
			constants::crashData
				.at(static_cast<std::size_t>(version))
				.first
				.at(static_cast<std::size_t>(crashType));
	} catch (...) {
		return "particle_10_001.png";
	}

	if (value)
		return value.value() ? "particle_50_001.png": "particle_193_001.png";
	
	return "particle_11_001.png";
}

void CrashtestCell::onCrash() {
	switch (static_cast<CrashType>(m_crashBtn->getTag())) {
		case CrashType::NULLPTR_DEREF:
			this->getChildByID("bwaaa")->getPosition();
			return;

		case CrashType::UTILS_TERMINATE:
			utils::terminate("Terminated");
			return;

		case CrashType::THROW:
			throw "Thrown";
			return;

		case CrashType::STD_EXCEPT: {
			std::string string = "String";
			auto character = string.at(55);
		}
		return;

		case CrashType::STD_ABORT:
			std::abort();
			return;

		case CrashType::COUNT:
			Notification::create("Reserved crash enum", NotificationIcon::Error, 3.f)->show();
			return;

		default:
			Notification::create("Unknown crash enum", NotificationIcon::Error, 3.f)->show();
			return;
	}
}