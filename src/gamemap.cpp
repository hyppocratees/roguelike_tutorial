#include "gamemap.h"
#include <vector>

GameMap::GameMap(int width, int height) : width_(width), height_(height)
{
	tiles_.reserve(width_ * height_);
	tiles_.assign(width_ * height_, tfloor);
	tiles_.at(22 * width_ + 30) = twall;
	tiles_.at(22 * width_ + 31) = twall;
	tiles_.at(22 * width_ + 32) = twall;
	console_ = tcod::Console(width_, height_);
}

bool GameMap::Inbound(int x, int y) const
{
	return 0 <= x && x < width_ && 0 <= y && y < height_;
}

void GameMap::Render(tcod::Console& cons)
{
	Update();
	tcod::blit(cons, console_);
}

void GameMap::Update() {
	for (int x = 0; x < width_; ++x) {
		for (int y = 0; y < height_; ++y) {
			console_.at(x, y).ch = tiles_.at(y * width_ + x).graphic.ch;
			console_.at(x, y).fg = tiles_.at(y * width_ + x).graphic.fg;
		}
	}
}