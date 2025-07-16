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
	graphic_tile graphic;
};

constexpr tile tfloor{ true, true, {'.', {255,255,255}, {50, 50, 150}} };

constexpr tile twall{ false, false, {'#', {255,255,255}, {0, 0, 100}} };