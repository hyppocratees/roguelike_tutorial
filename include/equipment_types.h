#pragma once
#include <memory>

enum EquipmentType {
	WEAPON,
	ARMOR
};

class Equipable {
public:
	Equipable(EquipmentType equiptype, int power_bonus, int defense_bonus) : equiptype_(equiptype), power_bonus_(power_bonus), defense_bonus_(defense_bonus) {};
	std::unique_ptr<Equipable> Clone() { return std::make_unique<Equipable>(*this); };
	int GetPowerBonus() const { return power_bonus_; };
	int GetDefenseBonus() const { return defense_bonus_; };
	EquipmentType GetEquipmentType() const { return equiptype_; };
private:
	int power_bonus_ = 0;
	int defense_bonus_ = 0;
	EquipmentType equiptype_;
};

static const Equipable NONE_W_EQUIP{ WEAPON, 0, 0 };
static const Equipable NONE_A_EQUIP{ ARMOR, 0, 0 };
static const Equipable DAGGER_EQUIP{ WEAPON, 2, 0 };
static const Equipable SWORD_EQUIP{ WEAPON, 4, 0 };
static const Equipable LEATHER_ARMOR_EQUIP{ ARMOR, 0, 1 };
static const Equipable CHAIN_MAIL_EQUIP{ ARMOR, 0, 3 };