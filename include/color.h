#pragma once

#include <libtcod/color.hpp>

constexpr tcod::ColorRGB white{ 0xFF, 0xFF, 0xFF };
constexpr tcod::ColorRGB black{ 0x0, 0x0, 0x0 };
constexpr tcod::ColorRGB player_atk{ 0xE0, 0xE0, 0xE0 };
constexpr tcod::ColorRGB enemy_atk{ 0xFF, 0xC0, 0xC0 };
constexpr tcod::ColorRGB player_die{ 0xFF, 0x30, 0x30 };
constexpr tcod::ColorRGB enemy_die{ 0xFF, 0xA0, 0x30 };
constexpr tcod::ColorRGB welcome_text{ 0x20, 0xA0, 0xFF };
constexpr tcod::ColorRGB bar_text = white;
constexpr tcod::ColorRGB bar_filled{ 0x0, 0x60, 0x0 };
constexpr tcod::ColorRGB bar_empty{ 0x40, 0x10, 0x10 };
constexpr tcod::ColorRGB invalid{ 0xFF, 0xFF, 0x00 };
constexpr tcod::ColorRGB impossible{ 0x80, 0x80, 0x80 };
constexpr tcod::ColorRGB error{ 0xFF, 0x40, 0x40 };
constexpr tcod::ColorRGB health_recovered{ 0x0, 0xFF, 0x0 };