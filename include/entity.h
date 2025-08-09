#pragma once
#include <libtcod/color.hpp>
#include <string>

class GameMap;

class Entity {
public:
	Entity() : x_(0), y_(0), char_(' '), color_({ 0, 0, 0 }), name_("<Unnamed>"), block_mov_(false), map_(nullptr) {};
	Entity(int x, int y, char c, tcod::ColorRGB color, std::string name, bool block_mov) : x_(x), y_(y), char_(c), color_(color), name_(name), block_mov_(block_mov) {};
	virtual void Move(int dx, int dy) { x_ += dx; y_ += dy; };
	virtual int GetX() const { return x_; };
	virtual int GetY() const { return y_; };
	virtual void SetX(int x) { x_ = x; };
	virtual void SetY(int y) { y_ = y; };
	virtual void SetChar(char c) { char_ = c; };
	virtual void SetBlockMove(bool bm) { block_mov_ = bm; };
	virtual void SetName(std::string name) { name_ = name; };
	virtual char GetChar() const { return char_; };
	virtual tcod::ColorRGB GetColor() const { return color_; };
	virtual std::string GetName() const { return name_; };
	virtual bool BlockMov() const { return block_mov_; };
	virtual void SetMap(GameMap* map) { map_ = map; };
	virtual GameMap* GetMap() const { return map_; };
protected:
	int x_;
	int y_;
	char char_;
	tcod::ColorRGB color_;
	std::string name_;
	bool block_mov_;
	GameMap* map_ = nullptr;
};