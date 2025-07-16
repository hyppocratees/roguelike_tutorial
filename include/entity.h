#pragma once
#include <libtcod/color.hpp>

class Entity {
public:
	Entity(int x, int y, char c, tcod::ColorRGB color) : x_(x), y_(y), char_(c), color_(color) {};
	void Move(int dx, int dy) { x_ += dx; y_ += dy; };
	int GetX() const { return x_; };
	int GetY() const { return y_; };
	char GetChar() const { return char_; };
	tcod::ColorRGB GetColor() const { return color_; };
private:
	int x_;
	int y_;
	char char_;
	tcod::ColorRGB color_;
};