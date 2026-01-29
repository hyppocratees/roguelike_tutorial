#include "item_manager.h"
#include "procgen.h"
#include "entity_factory.h"
#include "gameworld.h"

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


void ItemManager::PlaceEntities(RectangleRoom& room, int max_item_per_room, TCODRandom& random, const GameWorld& gw) {
	int num_item = random.getInt(0, max_item_per_room);

	std::vector<Item> items = gw.GetItemAtRandom(num_item);

	for (int i = 0; i < num_item; ++i) {
		int x = random.getInt(room.GetX1() + 1, room.GetX2() - 1);
		int y = random.getInt(room.GetY1() + 1, room.GetY2() - 1);

		Spawn(items.at(i), { x, y });
		
	}
}

void ItemManager::Clear() {
	items_.clear();
}

std::ostream& operator<<(std::ostream& os, const ItemManager& im)
{
	size_t size = im.items_.size();
	os << size << "\n";
	for (const Item& item : im.items_) {
		os << item;
	}
	return os;
}

std::istream& operator>>(std::istream& is, ItemManager& im)
{
	im.Clear();
	size_t size;
	is >> size;
	im.items_.reserve(size);
	for (auto i = 0; i < size; ++i) {
		Item item;
		is >> item;
		im.Spawn(item);
	}

	return is;
}
