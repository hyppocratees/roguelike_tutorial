#pragma once

#include <vector>
#include "item.h"

class Actor;

class Inventory {
public:
	Inventory(int capacity) : capacity_(capacity),  owner_(nullptr){}
	Inventory(int capacity, Actor* owner) : capacity_(capacity), owner_(owner) {}
	Inventory(const Inventory& inv) : capacity_(inv.capacity_), owner_(inv.owner_), inventory_(inv.inventory_) {}
	Item& drop(Item& item);
	size_t size() const { return inventory_.size(); };
	int capacity() const { return capacity_; };
	void add(Item& item) { inventory_.emplace_back(item); };
	void remove(Item& item);
	std::vector<Item>& GetInventory() { return inventory_; };
	void SetOwner(Actor* newowner) { owner_ = newowner; };
private:
	int capacity_;
	std::vector<Item> inventory_{};
	Actor* owner_;
};