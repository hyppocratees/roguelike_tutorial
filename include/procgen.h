#pragma once
#include <utility>
#include <vector>


#include "gamemap.h"

class Entity;


class RectangleRoom {
public:
	RectangleRoom(int x, int y, int width, int height) : x1_(x), x2_(x + height), y1_(y), y2_(y + width){};
	std::pair<int, int> Center() const;
	std::pair<std::vector<int>, std::vector<int>> Inner() const;
	bool Intersect(const RectangleRoom& room) const;
	int GetX1() const { return x1_; };
	int GetX2() const { return x2_; };
	int GetY1() const { return y1_; };
	int GetY2() const { return y2_; };
private:
	int x1_;
	int x2_;
	int y1_;
	int y2_;
};

class MapGenerator {
public:
	MapGenerator(int map_height, int map_width, int room_min_size, int room_max_size, int max_rooms);
	void Generate(GameMap& gamemap) const;
private:
	void GenerateRoom(GameMap& gamemap,const RectangleRoom& room) const;
	void GenerateTunnel(GameMap& gamemap, std::pair<int, int> start, std::pair<int, int> end) const;
	std::vector<std::pair<int, int>> TunnelBetween(std::pair<int, int> start, std::pair<int, int> end) const;
private:
	int map_height_;
	int map_width_;
	int room_min_size_;
	int room_max_size_;
	int max_rooms_;
};