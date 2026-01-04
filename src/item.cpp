#include <iostream>
#include <string>
#include "components/consumable.h"

#include "item.h"

std::ostream& operator<<(std::ostream& os, const Item& item) {
	os << item.name_ << "\n";
	os << item.char_ << "\n";
	os << item.x_ << " " << item.y_ << "\n";
	os << item.block_mov_ << "\n";
	os << item.rend_ord_ << "\n";
	os << item.color_ << "\n";
	if (item.consumable_) {
		os << "1" << "\n";
		os << *item.consumable_ << "\n";
	}
	else {
		os << "0" << "\n";
	}
	return os;
}

std::istream& operator>>(std::istream& is, Item& item) {
	is >> std::ws;
	std::getline(is, item.name_);
	is >> item.char_;
	is >> item.x_ >> item.y_;
	is >> item.block_mov_;
	is >> item.rend_ord_;
	is >> item.color_;
	bool has_consumable = false;
	is >> has_consumable;
	if (has_consumable) {
		int cons_type;
		int var1;
		int var2;
		is >> cons_type;
		switch (cons_type) {
		case 0:
			item.consumable_ = nullptr;
			break;
		case 1:
			is >> var1;
			item.consumable_ = std::make_unique<HealingConsumable>(var1);
			break;
		case 2:
			is >> var1 >> var2;
			item.consumable_ = std::make_unique<FireballConsumable>(var1, var2);
			break;
		case 3:
			is >> var1 >> var2;
			item.consumable_ = std::make_unique<LightningDamageConsumable>(var1, var2);
			break;
		case 4:
			is >> var1;
			item.consumable_ = std::make_unique<ConfusionConsumable>(var1);
			break;
		default:
			item.consumable_ = nullptr;
		}
	}


	return is;
}