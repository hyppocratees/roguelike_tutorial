#pragma once
#include <utility>
#include <vector>


#include "gamemap.h"


class RectangleRoom {
public:
	RectangleRoom(int x, int y, int width, int height) : x1_(x), x2_(x + width), y1_(y), y2_(y + height){};
	std::pair<int, int> Center() const;
	std::pair<std::vector<int>, std::vector<int>> Inner() const;
private:
	int x1_;
	int x2_;
	int y1_;
	int y2_;
};

class MapGenerator {
public:
	MapGenerator(int map_height, int map_width) : map_height_(map_height), map_width_(map_width) {};
	void Generate(GameMap& gamemap) const;
private:
	int map_height_;
	int map_width_;
};