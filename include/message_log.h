#pragma once
#include <string>
#include <libtcod/color.hpp>
#include <libtcod/libtcod.hpp>
#include <vector>
#include "color.h"

class Message {
public:
	Message(std::string text, tcod::ColorRGB color) : text_(text), color_(color), count_(1) {}
	std::string FullText() const;
	void IncreaseCount() { ++count_; };
	tcod::ColorRGB GetColor() const { return color_; };
private:
	std::string text_;
	tcod::ColorRGB color_;
	int count_;
};

class MessageLog {
public:
	MessageLog() {};
	void AddMessage(std::string text, tcod::ColorRGB color = white, bool stack = false);
	void Render(tcod::Console& console, int x, int y, int width, int height);
private:
	std::vector<Message> messages_{};
};