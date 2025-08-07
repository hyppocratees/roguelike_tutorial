#pragma once
#include <vector>
#include <utility>

class Actor;
class RectangleRoom;
class TCODRandom;

class EntityManager {
public:
    EntityManager() : entities_({}) {}

    Actor& Spawn(const Actor& src);
    Actor& Spawn(const Actor& src, std::pair<int, int> pos);
    void PlaceEntities(RectangleRoom& room, int max_monster_per_room, TCODRandom& random);
    Actor* GetBlockingEntity(int x, int y);
    Actor& Get(int index) { return entities_.at(index); };

    std::vector<Actor>::iterator begin()
    {
        return entities_.begin();
    }

    std::vector<Actor>::iterator end()
    {
        return entities_.end();
    }

    std::vector<Actor>::const_iterator begin() const
    {
        return entities_.begin();
    }

    std::vector<Actor>::const_iterator end() const
    {
        return entities_.end();
    }
private:
	std::vector<Actor> entities_;
};