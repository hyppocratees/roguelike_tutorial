#pragma once
#include "tile_types.h"
#include <libtcod/libtcod.hpp>
#include <vector>

class GameMap {
public:
	GameMap(int width, int height);
	bool Inbound(int x, int y) const;
	void Render(tcod::Console& cons);
	void Update();
	tile GetTile(int col, int line) const { return tiles_[line * width_ + col]; };
	void SetTile(int col, int line, tile tiletype) { tiles_[line * width_ + col] = tiletype; };
private:
	int width_;
	int height_;
	std::vector<tile> tiles_;
	tcod::Console console_;
};