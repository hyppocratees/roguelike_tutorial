#pragma once
#include <libtcod/color.hpp>

struct graphic_tile {
	char ch;
	tcod::ColorRGB fg;
	tcod::ColorRGB bg;
};

struct tile {
	bool walkable;
	bool transparent;
	graphic_tile dark;
	graphic_tile light;
	bool explored;
	unsigned short tile_type;
};

constexpr tile tfloor{ true, true, {'.', {255,255,255}, {50, 50, 150}}, {'.', {255,255,255}, {200, 180, 50}}, false, 0 };

constexpr tile twall{ false, false, {'#', {255,255,255}, {0, 0, 100}}, {'#', {255,255,255}, {130, 110, 50}}, false, 1 };

constexpr tile tshroud{ false, false, {' ', {0, 0, 0}, {0, 0, 0}}, {' ', {0, 0, 0}, {0, 0, 0}}, false, -1 };