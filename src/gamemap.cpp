#include "gamemap.h"
#include <vector>
#include <libtcod/fov.hpp>
#include "procgen.h"
#include <fstream>

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


std::ostream& operator<<(std::ostream& os, const GameMap& map) {
	// std::cout << "saving map" << std::endl;
	os << map.height_ << " " << map.width_ << std::endl;
	for (int i = 0; i < map.tiles_.size(); ++i) {
		int pos_y = i /map.width_;
		int pos_x = i - pos_y * map.width_;
		os << pos_x << " " << pos_y << " " << map.tiles_.at(i).tile_type << " " << map.tiles_.at(i).explored << std::endl;

	}
	return os;
}

std::istream& operator>>(std::istream& is, GameMap& map) {
	//std::cout << "reading map" << std::endl;
	is >> map.height_ >> map.width_;
	map.tiles_ = std::vector<tile>(map.width_ * map.height_, twall);
	for (int i = 0; i < map.width_ * map.height_; ++i) {
		int pos_y = 0;
		int pos_x = 0;
		is >> pos_x >> pos_y;
		int index = map.width_ * pos_y + pos_x;
		unsigned short tile_type;
		is >> tile_type;
		switch (tile_type) {
		case 0:
			map.tiles_.at(index) = tfloor; 
			map.map_.setProperties(pos_x, pos_y, tfloor.transparent, tfloor.walkable);
			break;
		case 2:
			map.tiles_.at(index) = tdownstairs;
			map.map_.setProperties(pos_x, pos_y, tdownstairs.transparent, tdownstairs.walkable);
			break;
		case 1:
		default:
			map.tiles_.at(index) = twall; 
			map.map_.setProperties(pos_x, pos_y, twall.transparent, twall.walkable);
		}
		
		is >> map.tiles_.at(index).explored;
		}
	
	return is;
}