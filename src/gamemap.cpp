#include "gamemap.h"
#include <vector>
#include <libtcod/fov.hpp>
#include "procgen.h"

GameMap::GameMap(int width, int height) : width_(width), height_(height), map_(width_, height_)
{
	tiles_.reserve(width_ * height_);
	tiles_.assign(width_ * height_, twall);
	console_ = tcod::Console(width_, height_);
}

GameMap::GameMap(const GameMap& gm) : width_(gm.width_), height_(gm.height_), map_(width_, height_), tiles_(gm.tiles_), console_(tcod::Console(width_, height_))
{
	for (int i = 0; i < width_; ++i) {
		for (int j = 0; j < height_; ++j) {
			map_.setProperties(i, j, gm.map_.isTransparent(i, j), gm.map_.isWalkable(i, j));
		}
	}
}

GameMap& GameMap::operator=(const GameMap& gm)
{
	GameMap out = GameMap(gm);
	return out;
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

bool GameMap::IsInFov(int col, int line) const
{
	return map_.isInFov(col, line);
}

void GameMap::UpdateFov(int x, int y, int radius)
{
	map_.computeFov(x, y, radius);
}

bool GameMap::IsExplored(int col, int line) const
{
	return tiles_.at(line * width_ + col).explored;
}

void GameMap::SetExplored(int col, int line, bool isexplored)
{
	tiles_.at(line * width_ + col).explored = isexplored;
}

void GameMap::AddRoom(RectangleRoom room)
{
	rooms_.push_back(room);
}

RectangleRoom& GameMap::GetRoom(int index)
{
	return rooms_.at(index);
}

void GameMap::Update() {
	for (int x = 0; x < width_; ++x) {
		for (int y = 0; y < height_; ++y) {
			if (IsInFov(x, y)) {
				tiles_.at(y * width_ + x).explored = true;
				console_.at(x, y).ch = tiles_.at(y * width_ + x).light.ch;
				console_.at(x, y).fg = tiles_.at(y * width_ + x).light.fg;
				continue;
			}
			if (IsExplored(x, y)) {
				console_.at(x, y).ch = tiles_.at(y * width_ + x).dark.ch;
				console_.at(x, y).fg = tiles_.at(y * width_ + x).dark.fg;
				continue;
			}
			console_.at(x, y).ch = tshroud.dark.ch;
			console_.at(x, y).fg = tshroud.dark.fg;
			
		}
	}
}

void GameMap::SetTile(int col, int line, tile tiletype)
{
	tiles_[line * width_ + col] = tiletype;
	map_.setProperties(col, line, tiletype.transparent, tiletype.walkable);
}


