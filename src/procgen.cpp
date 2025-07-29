#include <utility>
#include <vector>

#include "procgen.h"
#include "gamemap.h"

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

void MapGenerator::Generate(GameMap& gamemap) const
{
	RectangleRoom room1(20, 15, 10, 15);
	RectangleRoom room2(35, 15, 10, 15);
	std::pair<std::vector<int>, std::vector<int>> inner1 = room1.Inner();
	std::pair<std::vector<int>, std::vector<int>> inner2 = room2.Inner();
	for (int x : inner1.first) {
		for (int y : inner1.second) {
			gamemap.SetTile(x, y, tfloor);
		}
	}
	for (int x : inner2.first) {
		for (int y : inner2.second) {
			gamemap.SetTile(x, y, tfloor);
		}
	}
}
