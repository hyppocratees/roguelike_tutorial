#include <utility>
#include <vector>

#include <libtcod/bresenham.hpp>
#include <libtcod/mersenne.hpp>

#include "procgen.h"
#include "gamemap.h"
#include "entity.h"

std::pair<int, int> RectangleRoom::Center() const
{
	int center_x = (int)(x1_ + x2_) / 2;
	int center_y = (int)(y1_ + y2_) / 2;
	return std::pair<int, int>(center_x, center_y);
}

std::pair<std::vector<int>, std::vector<int>> RectangleRoom::Inner() const
{
	std::vector<int> xslice;
	std::vector<int> yslice;
	for (int x = x1_ + 1; x < x2_; ++x) xslice.emplace_back(x);
	for (int y = y1_ + 1; y < y2_; ++y) yslice.emplace_back(y);
	return std::pair<std::vector<int>, std::vector<int>>(xslice, yslice);
}

bool RectangleRoom::Intersect(const RectangleRoom& room) const
{
	return x1_ <= room.x1_ && x2_ >= room.x1_ && y1_ <= room.y1_ && y2_ >= room.y1_;
}

void MapGenerator::GenerateRoom(GameMap& gamemap, const RectangleRoom& room) const
{
	std::pair<std::vector<int>, std::vector<int>> inner = room.Inner();
	for (int x : inner.first) {
		for (int y : inner.second) {
			gamemap.SetTile(x, y, tfloor);
		}
	}
}

std::vector<std::pair<int, int>> MapGenerator::TunnelBetween(std::pair<int, int> start, std::pair<int, int> end) const
{
	int cornerx;
	int cornery;
	TCODRandom random;
	if (random.getFloat(0, 1) < 0.5f) {
		cornerx = end.first;
		cornery = start.second;
	}
	else {
		cornerx = start.first;
		cornery = end.second;
	}
	
	tcod::BresenhamLine line1({ start.first, start.second }, { cornerx, cornery });
	tcod::BresenhamLine line2({ cornerx, cornery }, { end.first, end.second });

	std::vector<std::pair<int, int>> tunnel;

	for (auto p : line1) {
		int x = p.at(0);
		int y = p.at(1);
		tunnel.emplace_back(x, y);
	}
	for (auto p : line2) {
		int x = p.at(0);
		int y = p.at(1);
		tunnel.emplace_back(x, y);
	}

	return tunnel;
}

void MapGenerator::GenerateTunnel(GameMap& gamemap, std::pair<int, int> start, std::pair<int, int> end) const
{
	for (std::pair<int, int> point : TunnelBetween(start, end)) {
		gamemap.SetTile(point.first, point.second, tfloor);
	}
}

MapGenerator::MapGenerator(int map_height, int map_width, int room_min_size, int room_max_size, int max_rooms) : map_height_(map_height), map_width_(map_width), room_min_size_(room_min_size), room_max_size_(room_max_size), max_rooms_(max_rooms)
{
}

void MapGenerator::Generate(GameMap& gamemap) const
{
	gamemap.Reset();
	std::pair<int, int> center_of_last_room{ 0,0 };
	std::vector<RectangleRoom> rooms{};
	TCODRandom random;

	for (int i = 0; i < max_rooms_; ++i) {
		int room_width = random.getInt(room_min_size_, room_max_size_);
		int room_height = random.getInt(room_min_size_, room_max_size_);
		int y = random.getInt(0, map_width_ - room_width - 1);
		int x = random.getInt(0, map_height_ - room_height - 1);
		RectangleRoom room(x, y, room_width, room_height);
		bool intersect = false;
		for (RectangleRoom& r : rooms) {
			if (room.Intersect(r)) {
				intersect = true;
				break;
			}
		}
		if (intersect) continue;

		GenerateRoom(gamemap, room);
		if (rooms.size() == 0) {
			int px, py;
			px = room.Center().first;
			py = room.Center().second;
		}
		else {
			GenerateTunnel(gamemap, room.Center(), rooms.back().Center());
		}
		rooms.push_back(room);
		gamemap.AddRoom(room);
		center_of_last_room = room.Center();
	}
	gamemap.SetTile(center_of_last_room.first, center_of_last_room.second, tdownstairs);
	gamemap.SetDownStairLocation(center_of_last_room);
}
