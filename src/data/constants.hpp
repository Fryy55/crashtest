#pragma once

#include <array>
#include "enums.hpp"


namespace constants {
	using VersionData = std::pair<
		std::array<
			std::optional<bool>,
			static_cast<std::size_t>(CrashType::COUNT)
		>,
		std::array<
			std::optional<bool>,
			static_cast<std::size_t>(CrashType::COUNT)
		>
	>;

	inline constexpr auto unknown = std::nullopt;

	inline constexpr std::array<VersionData, static_cast<std::size_t>(GeodeVersion::COUNT)> crashData {
		VersionData

		// v4.6.2
		{
			{ false, false, false, false, false }, // Direct
			{ true, true, true, true, false } // Scheduled
		},
		// v4.6.3
		{
			{ true, true, true, true, false }, // Direct
			{ true, true, true, true, false } // Scheduled
		}

		/* // vX.Y.Z
		{
			{ unknown, unknown, unknown, unknown, unknown }, // Direct
			{ unknown, unknown, unknown, unknown, unknown } // Scheduled
		}, */
	};
}