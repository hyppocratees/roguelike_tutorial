#include "gameworld.h"
#include "gamemap.h"
#include "procgen.h"
#include "engine.h"

#include <iostream>

void GameWorld::GenerateFloor(GameMap& map)
{
	current_floor_++;
	pmap_gen_->Generate(map);
}

void GameWorld::PlaceEntities(Engine& engine, bool setplayer)
{
	TCODRandom random;
	GameMap* map = engine.GetMap();
	EntityManager& entities = engine.GetEntities();
	ItemManager& items = engine.GetItem();
	std::vector<RectangleRoom>& rooms = map->GetRooms();
	for (RectangleRoom& room : rooms) {
		entities.PlaceEntities(room, GetMaxValueForFloor(max_monster_per_room_), random, *this);
		items.PlaceEntities(room, GetMaxValueForFloor(max_item_per_room_), random, *this);
	}
	for (Entity& entity : entities) entity.SetMap(map);
	for (Item& item : items) item.SetMap(map);
	if (setplayer) engine.SetPlayer();
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

int GameWorld::GetMaxValueForFloor(std::vector<std::array<int, 2>> chance) const {
	int c_val = chance.at(0).at(1);

	for (auto& ar : chance) {
		if (ar.at(0) > current_floor_) {
			break;
		}
		c_val = ar.at(1);
	}
	return c_val;
}


std::vector<Actor> GameWorld::GetEntityAtRandom(int number_of_entity) const
{
	auto* rand = TCODRandom::getInstance();

	std::map<Actor, int> entity_weighted_chances;

	for (const auto& [key, val] : enemy_chance_) {
		// std::cout << key << std::endl;
		if (key > current_floor_) {
			break;
		}
		for (auto& value : val) {
			Actor entity = value.first;
			int weighted_chance = value.second;
			entity_weighted_chances[entity] = weighted_chance;
		}
	}

	std::vector<Actor> v_ac;
	std::vector<int> v_chance;

	int s_chance = 0;

	for (const auto& [key, val] : entity_weighted_chances) {
		v_ac.emplace_back(key);
		v_chance.emplace_back(val);
		s_chance += val;
	}

	int c;
	std::vector<Actor> chosen_entity;

	for (int i = 0; i < number_of_entity; ++i) {
		c = rand->getInt(0, s_chance);
		int c_chance = 0;
		for (int j = 0; j < v_ac.size(); ++j) {
			c_chance += v_chance.at(j);
			if (c <= c_chance) chosen_entity.emplace_back(v_ac.at(j));
		}
	}

	return chosen_entity;
}

std::vector<Item> GameWorld::GetItemAtRandom(int number_of_item) const
{
	auto* rand = TCODRandom::getInstance();

	std::map<Item, int> item_weighted_chances;

	for (const auto& [key, val] : item_chance_) {
		// std::cout << key << std::endl;
		if (key > current_floor_) {
			break;
		}
		for (auto& value : val) {
			Item entity = value.first;
			int weighted_chance = value.second;
			item_weighted_chances[entity] = weighted_chance;
		}
	}

	std::vector<Item> v_ac;
	std::vector<int> v_chance;

	int s_chance = 0;

	for (const auto& [key, val] : item_weighted_chances) {
		v_ac.emplace_back(key);
		v_chance.emplace_back(val);
		s_chance += val;
	}

	int c;
	std::vector<Item> chosen_item;

	for (int i = 0; i < number_of_item; ++i) {
		c = rand->getInt(0, s_chance);
		int c_chance = 0;
		for (int j = 0; j < v_ac.size(); ++j) {
			c_chance += v_chance.at(j);
			if (c <= c_chance) chosen_item.emplace_back(v_ac.at(j));
		}
	}

	return chosen_item;
}

