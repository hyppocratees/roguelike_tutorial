#pragma once
#include <vector>
#include <utility>

class Entity;
class RectangleRoom;

class EntityManager {
public:
    EntityManager() : entities_({}) {}

    Entity& Spawn(const Entity& src);
    Entity& Spawn(const Entity& src, std::pair<int, int> pos);
    void PlaceEntities(RectangleRoom& room, int max_monster_per_room);
    Entity* GetBlockingEntity(int x, int y);
    Entity& Get(int index) { return entities_.at(index); };

    std::vector<Entity>::iterator begin()
    {
        return entities_.begin();
    }

    std::vector<Entity>::iterator end()
    {
        return entities_.end();
    }

    std::vector<Entity>::const_iterator begin() const
    {
        return entities_.begin();
    }

    std::vector<Entity>::const_iterator end() const
    {
        return entities_.end();
    }
private:
	std::vector<Entity> entities_;
};