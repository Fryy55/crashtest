#pragma once

#include <Geode/Geode.hpp>

#include "enums.hpp"


class CrashtestShelf final : public cocos2d::CCMenu {
public:
	enum Type : std::uint8_t {
		DIRECT,
		SCHEDULED
	};

	static CrashtestShelf* create(Type type, GeodeVersion defaultVersion);

	void addCells(GeodeVersion version);
	void readdCells(GeodeVersion newVersion);

private:
	bool init(Type, GeodeVersion);

	cocos2d::CCLabelBMFont* m_label;
	cocos2d::CCMenu* m_menu;
	Type m_shelfType;
};