#pragma once
#include <algorithm>
#include "components/base_component.h"

#include <iostream>

class Actor;

class Fighter : public BaseComponent {
public:
	Fighter() :max_hp_(0), hp_(0), power_(0), defense_(0) {};
	Fighter(int maxhp, int defense, int power) : max_hp_(maxhp), hp_(max_hp_), power_(power), defense_(defense) {};
	Fighter(int hp, int max_hp, int defense, int power, Entity* entity) : hp_(hp), max_hp_(max_hp), defense_(defense), power_(power), BaseComponent(entity) {};
	int GetHp() const { return hp_; };
	int GetMaxHp() const { return max_hp_; };
	int Hp() const { return hp_; };
	void Hp(int value) {hp_ = std::max(0, std::min(value, max_hp_));};
	void SetEntity(Entity* entity) { entity_ = entity; };
	int GetPower() const { return power_; };
	int GetDefense() const { return defense_; };
	void Die(Actor& actor);
	int Heal(int amount);
	void TakeDamage(int amount) { hp_ -= amount; };
	void GainMaxHp(int amount) { max_hp_ += amount; };
	void GainPower(int amount) { power_ += amount; };
	void GainDefense(int amount) { defense_ += amount; };

	friend std::ostream& operator<<(std::ostream& os, const Fighter& fighter);
	friend std::istream& operator>>(std::istream& is, Fighter& fighter);
private:
	int max_hp_;
	int hp_; 
	int defense_; 
	int power_;
};