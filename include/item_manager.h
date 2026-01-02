#pragma once

#include "item.h"

#include <vector>

class RectangleRoom;
class TCODRandom;

class ItemManager {
public:
    ItemManager() : items_({}) {}

    Item& Spawn(const Item& src);
    Item& Spawn(const Item& src, std::pair<int, int> pos);
    void PlaceEntities(RectangleRoom& room, int max_item_per_room, TCODRandom& random);
    Item& Get(int index) { return items_.at(index); };
    std::vector<Item> Get() { return items_; };
    void Remove(Item& item) { items_.erase(std::remove(items_.begin(), items_.end(), item)); };

    std::vector<Item>::iterator begin()
    {
        return items_.begin();
    }

    std::vector<Item>::iterator end()
    {
        return items_.end();
    }

    std::vector<Item>::const_iterator begin() const
    {
        return items_.begin();
    }

    std::vector<Item>::const_iterator end() const
    {
        return items_.end();
    }

    friend std::ostream& operator<<(std::ostream& os, const ItemManager& im);
    friend std::istream& operator>>(std::istream& is, ItemManager& im);

private:
	std::vector<Item> items_;
};