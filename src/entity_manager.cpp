#include "entity_manager.h"
#include "entity.h"
#include <vector>
#include <libtcod/mersenne.hpp>
#include "procgen.h"
#include "entity_factory.h"

Entity& EntityManager::Spawn(const Entity& src)
{
	return entities_.emplace_back(src);
}

Entity& EntityManager::Spawn(const Entity& src, std::pair<int, int> pos)
{
	Entity& entity = Spawn(src);
	if (pos.first != entity.GetX() && pos.second != entity.GetY()) {
		entity.SetX(pos.first);
		entity.SetY(pos.second);
	}

	return entity;
}

void EntityManager::PlaceEntities(RectangleRoom& room, int max_monster_per_room)
{
	TCODRandom random;
	int num_monster = random.getInt(0, max_monster_per_room);
	for (int i = 0; i < num_monster; ++i) {
		int x = random.getInt(room.GetX1() + 1, room.GetX2() - 1);
		int y = random.getInt(room.GetY1() + 1, room.GetY2() - 1);

		if (GetBlockingEntity(x, y)) continue;

		if (random.getFloat(0, 1) < 0.8) Spawn(ORC, {x, y}); 
		else Spawn(TROLL, {x, y});
	}
}

Entity* EntityManager::GetBlockingEntity(int x, int y)
{
	for (auto& entity : entities_) {
		if (entity.GetX() == x && entity.GetY() == y && entity.BlockMov()) return &entity;
	}
	return nullptr;
}
