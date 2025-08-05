#pragma once
#include <algorithm>
#include "components/base_component.h"

class Fighter : public BaseComponent {
public:
	Fighter(int hp, int max_hp, int defense, int power, Entity& entity) : hp_(hp), max_hp_(max_hp), defense_(defense), power_(power), BaseComponent(entity) {}
	int GetHp() const { return hp_; };
	int Hp() const { return hp_; };
	int Hp(int value) { hp_ = std::max(0, std::min(value, max_hp_)); };
private:
	int max_hp_;
	int hp_; 
	int defense_; 
	int power_;
};