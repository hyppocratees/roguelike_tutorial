#pragma once

#include "gamemap.h"
#include "procgen.h"

#include <iostream>

class GameWorld {
public:
	GameWorld(int map_width, int map_height, int room_min_size, int room_max_size, int max_rooms) : pmap_gen_(std::make_unique<MapGenerator>(map_width, map_height,  room_min_size, room_max_size, max_rooms)) {};
	GameWorld(const GameWorld& gw) : current_floor_(gw.current_floor_), pmap_gen_(std::make_unique<MapGenerator>(*gw.pmap_gen_)) {};
	void GenerateFloor(GameMap& map);
	int GetDungeonLevel() const { return current_floor_; };

	friend std::ostream& operator<<(std::ostream& os, const GameWorld& gw);
	friend std::istream& operator>>(std::istream& is, GameWorld& gw);

private:
	std::unique_ptr<MapGenerator> pmap_gen_;
	int current_floor_ = 0;
};