#pragma once

#include "gamemap.h"
#include "procgen.h"
#include "entity_factory.h"

#include <iostream>
#include <vector>
#include <map>

class Engine;

class GameWorld {
public:
	GameWorld(int map_width, int map_height, int room_min_size, int room_max_size, int max_rooms) : pmap_gen_(std::make_unique<MapGenerator>(map_width, map_height,  room_min_size, room_max_size, max_rooms)) {
		item_chance_ = { { 0 , {{HEALTH_POTION, 35 }} }, { 2, {{CONFUSION_SCROLL, 10}} }, { 4, {{LIGHTNING_SCROLL, 25}} }, { 6, {{FIREBALL_SCROLL, 25}} } };
		enemy_chance_ = { { 0, { {ORC, 80 } } }, { 3, {{TROLL, 15}} }, { 5, {{TROLL, 30}} }, { 7, {{TROLL, 60}} } };
	};
	GameWorld(const GameWorld& gw) : current_floor_(gw.current_floor_), pmap_gen_(std::make_unique<MapGenerator>(*gw.pmap_gen_)), item_chance_(gw.item_chance_), enemy_chance_(gw.enemy_chance_) {};
	void GenerateFloor(GameMap& map);
	void PlaceEntities(Engine& engine, bool setplayer = true);
	int GetDungeonLevel() const { return current_floor_; };
	std::vector<Actor> GetEntityAtRandom(int number_of_entity) const;
	std::vector<Item> GetItemAtRandom(int number_of_item) const;

	friend std::ostream& operator<<(std::ostream& os, const GameWorld& gw);
	friend std::istream& operator>>(std::istream& is, GameWorld& gw);
private:
	int GetMaxValueForFloor(std::vector<std::array<int, 2>> chance) const;
private:
	std::unique_ptr<MapGenerator> pmap_gen_;
	int current_floor_ = 0;
	std::map<int, std::vector<std::pair<Item, int>>> item_chance_{};
	std::map<int, std::vector<std::pair<Actor, int>>> enemy_chance_{};	
	std::vector<std::array<int, 2>> max_monster_per_room_{ { {1, 2}, {4, 3}, {6, 5} } };;
	std::vector<std::array<int, 2>> max_item_per_room_{ { {1, 1}, {4, 2} } };
};