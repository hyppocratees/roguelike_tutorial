#pragma once

#include "base_component.h"
#include <memory>

class Entity;
class Actor;
class Action;
class ItemAction;
class Engine;

class Consumable : public BaseComponent{
public:
	Consumable() : BaseComponent() {};
	Consumable(Entity* pentity) : BaseComponent(pentity) {};
	virtual std::unique_ptr<Action> GetAction(Actor& consumer) const;
	virtual bool Activate(Engine& engine, const ItemAction const* action) const = 0;
	virtual std::unique_ptr<Consumable> Clone() = 0;
};

class HealingConsumable : public Consumable {
public:
	HealingConsumable(int amount) : Consumable(), amount_(amount) {}
	HealingConsumable(int amount, Entity* entity) : Consumable(entity), amount_(amount) {}
	virtual bool Activate(Engine& engine, const ItemAction const* action) const;
	virtual std::unique_ptr<Consumable> Clone() { return std::make_unique<HealingConsumable>(amount_); };
private:
	int amount_;
};

class LightningDamageConsumable : public Consumable {
public:
	LightningDamageConsumable(int damage, int max_range) : Consumable(), damage_(damage), max_range_(max_range) {};
	virtual std::unique_ptr<Consumable> Clone() { return std::make_unique<LightningDamageConsumable>(damage_, max_range_); };
	virtual bool Activate(Engine& engine, const ItemAction const* action) const;
private:
	int damage_;
	int max_range_;
};