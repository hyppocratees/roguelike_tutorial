#pragma once

#include <vector>
#include "item.h"

class Actor;

class Inventory {
public:
	Inventory(int capacity) : capacity_(capacity),  owner_(nullptr){}
	Inventory(int capacity, Actor* owner) : capacity_(capacity), owner_(owner) {}
	Inventory(const Inventory& inv) : capacity_(inv.capacity_), owner_(inv.owner_), inventory_(inv.inventory_) {}
	Item& Drop(Item& item);
	size_t Size() const { return inventory_.size(); };
	int Capacity() const { return capacity_; };
	void Add(Item& item) { inventory_.emplace_back(item); };
	void Remove(Item& item);
	std::vector<Item>& GetInventory() { return inventory_; };
	void SetOwner(Actor* newowner) { owner_ = newowner; };
	Item& operator[](int index) { return inventory_.at(index); };
private:
	int capacity_;
	std::vector<Item> inventory_{};
	Actor* owner_;
};