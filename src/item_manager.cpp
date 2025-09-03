#include "item_manager.h"
#include "procgen.h"
#include "entity_factory.h"

#include <utility>
#include <libtcod/mersenne.hpp>

Item& ItemManager::Spawn(const Item& src) {
	return items_.emplace_back(src);
}

Item& ItemManager::Spawn(const Item& src, std::pair<int, int> pos) {
	Item& item = Spawn(src);
	if (item.GetX() != pos.first || item.GetY() == pos.second) {
		item.SetX(pos.first);
		item.SetY(pos.second);
	}
	return item;
}


void ItemManager::PlaceEntities(RectangleRoom& room, int max_item_per_room, TCODRandom& random) {
	int num_monster = random.getInt(0, max_item_per_room);
	for (int i = 0; i < num_monster; ++i) {
		int x = random.getInt(room.GetX1() + 1, room.GetX2() - 1);
		int y = random.getInt(room.GetY1() + 1, room.GetY2() - 1);

		float item_chance = random.getFloat(0, 1);
		if (item_chance <= 0.7) Spawn(HEALTH_POTION, { x, y });
		else Spawn(LIGHTNING_SCROLL, { x, y });
		
	}
}