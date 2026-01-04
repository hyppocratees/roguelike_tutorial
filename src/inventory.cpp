#include "inventory.h"
#include "item.h"

#include <vector>

Item& Inventory::Drop(Item& item)
{
	inventory_.erase(std::find(inventory_.begin(), inventory_.end(), item));
	return item;
}

void Inventory::Remove(Item& item)
{
	auto it = std::find(inventory_.begin(), inventory_.end(), item);
	if (it != inventory_.end()) {
		inventory_.erase(it);
	}
}

std::ostream& operator<<(std::ostream& os, const Inventory& inv)
{
	os << inv.capacity_ << "\n";
	int num_elem = inv.inventory_.size();
	os << num_elem << "\n";
	for (int i = 0; i < num_elem; ++i) {
		os << inv.inventory_.at(i);
	}
	return os;
}

std::istream& operator>>(std::istream& is, Inventory& inv)
{
	is >> inv.capacity_;
	inv.inventory_.reserve(inv.capacity_);
	int num_elem = 0;
	is >> num_elem;
	for (int i = 0; i < num_elem; ++i) {
		Item item;
		is >> item;
		inv.inventory_.emplace_back(item);
	}
	return is;
}
