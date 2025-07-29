#pragma once
#include "tile_types.h"
#include <libtcod/libtcod.hpp>
#include <vector>
#include <libtcod/fov.hpp>

class GameMap {
public:
	GameMap(int width, int height);
	bool Inbound(int x, int y) const;
	void Render(tcod::Console& cons);
	void Update();
	tile GetTile(int col, int line) const { return tiles_[line * width_ + col]; };
	void SetTile(int col, int line, tile tiletype);
	bool IsInFov(int col, int line) const;
	void UpdateFov(int x, int y, int radius);
	bool IsExplored(int col, int line) const;
	void SetExplored(int col, int line, bool isexplored);
private:
	int width_;
	int height_;
	std::vector<tile> tiles_;
	tcod::Console console_;
	TCODMap map_;
};