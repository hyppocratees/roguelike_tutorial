#pragma once
#include "tile_types.h"
#include <libtcod/libtcod.hpp>
#include <libtcod/fov.hpp>
#include <fstream>
#include <vector>
#include <utility>

class RectangleRoom;

class GameMap {
public:
	GameMap(int width, int height);
	GameMap(const GameMap& gm);
	GameMap& operator=(const GameMap& gm);

	bool Inbound(int x, int y) const;
	void Render(tcod::Console& cons);
	void Update();
	tile GetTile(int col, int line) const { return tiles_[line * width_ + col]; };
	void SetTile(int col, int line, tile tiletype);
	bool IsInFov(int col, int line) const;
	void UpdateFov(int x, int y, int radius);
	bool IsExplored(int col, int line) const;
	void SetExplored(int col, int line, bool isexplored);
	void AddRoom(RectangleRoom room);
	RectangleRoom& GetRoom(int index);
	std::vector<RectangleRoom>& GetRooms() { return rooms_; };
	TCODMap* GetMap() { return &map_; };
	int GetWidth() const { return width_; };
	int GetHeight() const { return height_; };
	std::pair<int, int> GetDownStairLocation() const { return downstair_loc_; }
	friend std::ostream& operator<<(std::ostream& os, const GameMap& map);
	friend std::istream& operator>>(std::istream& is, GameMap& map);
	void SetDownStairLocation(std::pair<int, int>loc) { downstair_loc_ = loc; };
	void Reset();

private:
	int width_;
	int height_;
	std::vector<tile> tiles_;
	std::vector<RectangleRoom> rooms_{};
	tcod::Console console_;
	TCODMap map_;
	std::pair<int, int> downstair_loc_{ 0, 0 };
};