#pragma once
#include <libtcod/color.hpp>
#include <string>

class Entity {
public:
	Entity() : x_(0), y_(0), char_(' '), color_({ 0, 0, 0 }), name_("<Unnamed>"), block_mov_(false) {};
	Entity(int x, int y, char c, tcod::ColorRGB color, std::string name, bool block_mov) : x_(x), y_(y), char_(c), color_(color), name_(name), block_mov_(block_mov) {};
	void Move(int dx, int dy) { x_ += dx; y_ += dy; };
	int GetX() const { return x_; };
	int GetY() const { return y_; };
	void SetX(int x) { x_ = x; };
	void SetY(int y) { y_ = y; };
	char GetChar() const { return char_; };
	tcod::ColorRGB GetColor() const { return color_; };
	std::string GetName() const { return name_; };
	bool BlockMov() const { return block_mov_; };
private:
	int x_;
	int y_;
	char char_;
	tcod::ColorRGB color_;
	std::string name_;
	bool block_mov_;
};