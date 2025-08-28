#include "inventory.h"
#include "item.h"

#include <vector>

Item& Inventory::drop(Item& item)
{
	inventory_.erase(std::find(inventory_.begin(), inventory_.end(), item));
	return item;
}

void Inventory::remove(Item& item)
{
	auto it = std::find(inventory_.begin(), inventory_.end(), item);
	if (it != inventory_.end()) {
		inventory_.erase(it);
	}
}
