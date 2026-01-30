#pragma once

#include "entity.h"
#include "components/consumable.h"
#include "equipment_types.h"
#include <memory>

class Item : public Entity {
public:
	Item() : Entity(), consumable_(nullptr) {};
	Item(int x, int y, char c, tcod::ColorRGB color, std::string name, bool block_mov, int rend_ord, std::unique_ptr<Consumable> consumable) : Entity(x, y, c, color, name, block_mov, rend_ord), consumable_(consumable->Clone()) {
		if (consumable_) consumable_->SetEntity(this);
	};
	Item(int x, int y, char c, tcod::ColorRGB color, std::string name, bool block_mov, int rend_ord, std::unique_ptr<Equipable> equipable) : Entity(x, y, c, color, name, block_mov, rend_ord), consumable_(nullptr), equipable_(equipable->Clone()) {
		if (consumable_) consumable_->SetEntity(this);
	};
	Item(int x, int y, char c, tcod::ColorRGB color, std::string name, bool block_mov, int rend_ord, std::unique_ptr<Consumable> consumable, std::unique_ptr<Equipable> equipable) : Entity(x, y, c, color, name, block_mov, rend_ord), consumable_(consumable->Clone()), equipable_(equipable->Clone()) {
		if (consumable_) consumable_->SetEntity(this);
	};
	Item(const Item& item) : Entity(item.x_, item.y_, item.char_, item.color_, item.name_, item.block_mov_, item.rend_ord_), consumable_(item.consumable_->Clone()), equipable_(item.equipable_->Clone()) {
		if (consumable_) consumable_->SetEntity(this);
	};
	Item& operator=(const Item& other) {
		if (this == &other) return *this;
		x_ = other.x_; 
		y_ = other.y_;
		char_ = other.char_;
		color_ = other.color_;
		name_ = other.name_;
		block_mov_ = other.block_mov_;
		map_ = other.map_; //Item does not own map
		rend_ord_ = other.rend_ord_;
		if (other.consumable_){
			consumable_ = std::move(other.consumable_->Clone());
			consumable_->SetEntity(this);
		}
		else {
			consumable_ = nullptr;
		}
		if (other.equipable_) {
			equipable_ = other.equipable_->Clone();
		}
		return *this;
	}

	Consumable* GetConsumable() { return consumable_.get(); };
	friend bool operator==(const Item& lhs, const Item& rhs)
	{
		return &lhs == &rhs;
	};

	bool IsConsumable() const { return consumable_ != nullptr; };

	friend std::ostream& operator<<(std::ostream& os, const Item& item);
	friend std::istream& operator>>(std::istream& is, Item& item);

private:
	std::unique_ptr<Consumable> consumable_ = nullptr;
	std::unique_ptr<Equipable> equipable_ = nullptr;
};