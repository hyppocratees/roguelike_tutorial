#pragma once

#include "base_component.h"
#include <memory>

class Entity;
class Actor;
class Item;
class Action;
class ItemAction;
class Engine;

class Consumable : public BaseComponent{
public:
	Consumable() : BaseComponent() {};
	Consumable(Entity* pentity) : BaseComponent(pentity) {};
	virtual std::unique_ptr<Action> GetAction(Actor& consumer) const;
	virtual std::unique_ptr<Action> GetAction(Engine& engine, Actor& consumer, Item& item) const;
	virtual bool Activate(Engine& engine, const ItemAction *const action) const = 0;
	virtual std::unique_ptr<Consumable> Clone() = 0;	

	friend std::ostream& operator<<(std::ostream& os, const Consumable& ai);

	virtual void ToText(std::ostream& os) const;
};

class HealingConsumable : public Consumable {
public:
	HealingConsumable(int amount) : Consumable(), amount_(amount) {}
	HealingConsumable(int amount, Entity* entity) : Consumable(entity), amount_(amount) {}
	virtual bool Activate(Engine& engine, const ItemAction *const action) const;
	virtual std::unique_ptr<Consumable> Clone() { return std::make_unique<HealingConsumable>(amount_); };

	virtual void ToText(std::ostream& os) const;
private:
	int amount_;
};

class LightningDamageConsumable : public Consumable {
public:
	LightningDamageConsumable(int damage, int max_range) : Consumable(), damage_(damage), max_range_(max_range) {};
	virtual std::unique_ptr<Consumable> Clone() { return std::make_unique<LightningDamageConsumable>(damage_, max_range_); };
	virtual bool Activate(Engine& engine, const ItemAction *const action) const;

	virtual void ToText(std::ostream& os) const;
private:
	int damage_;
	int max_range_;
};

class ConfusionConsumable : public Consumable {
public:
	ConfusionConsumable(int n_turn) : Consumable(), number_turn_(n_turn) {};
	virtual std::unique_ptr<Consumable> Clone() { return std::make_unique<ConfusionConsumable>(number_turn_); };
	virtual bool Activate(Engine& engine, const ItemAction *const action) const;
	virtual std::unique_ptr<Action> GetAction(Engine& engine, Actor& consumer, Item& item) const;

	virtual void ToText(std::ostream& os) const;
private:
	int number_turn_;
};

class FireballConsumable : public Consumable {
public:
	FireballConsumable(int damage, int radius) : damage_(damage), radius_(radius) {};
	virtual std::unique_ptr<Consumable> Clone() { return std::make_unique<FireballConsumable>(damage_, radius_); };
	virtual bool Activate(Engine& engine, const ItemAction *const action) const;
	virtual std::unique_ptr<Action> GetAction(Engine& engine, Actor& consumer, Item& item) const;

	virtual void ToText(std::ostream& os) const;
private:
	int damage_;
	int radius_;
};