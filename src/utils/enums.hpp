#pragma once


enum class GeodeVersion : std::uint8_t {
	V4_6_2,
	V4_6_3,

	COUNT
};
inline GeodeVersion& operator++(GeodeVersion& version) {
	return version = static_cast<GeodeVersion>((static_cast<std::uint8_t>(version) + 1));
}
inline GeodeVersion& operator--(GeodeVersion& version) {
	return version = static_cast<GeodeVersion>((static_cast<std::uint8_t>(version) - 1));
}

enum class CrashType : std::uint8_t {
	NULLPTR_DEREF,
	UTILS_TERMINATE,
	THROW,
	STD_EXCEPT,
	STD_ABORT,

	COUNT
};