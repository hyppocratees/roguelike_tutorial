#include "gameworld.h"
#include "gamemap.h"
#include "procgen.h"

#include <iostream>

void GameWorld::GenerateFloor(GameMap& map)
{
	current_floor_++;
	pmap_gen_->Generate(map);
}

std::ostream& operator<<(std::ostream& os, const GameWorld& gw)
{
	os << gw.current_floor_ << std::endl;
	os << *gw.pmap_gen_ << std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, GameWorld& gw)
{
	is >> gw.current_floor_;
	int map_height;
	int map_width;
	int room_min_size;
	int room_max_size;
	int max_rooms;
	is >> map_height >> map_width >> max_rooms >> room_min_size >> room_max_size;
	gw.pmap_gen_ = std::make_unique<MapGenerator>(map_height, map_width, room_min_size, room_max_size, max_rooms);
	return is;
}
