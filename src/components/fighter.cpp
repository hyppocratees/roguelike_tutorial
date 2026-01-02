#include "components/fighter.h"
#include "actor.h"

#include <format>
#include <iostream>
#include <string>
#include <algorithm>

void Fighter::Die(Actor& actor) {
	entity_->SetChar('%');
	entity_->SetBlockMove(false);
	actor.SetAI(std::make_unique<DeadAI>(&actor));
	entity_->SetName(std::format("remains of {}", entity_->GetName()));
	entity_->SetRendOrd(0);

}

int Fighter::Heal(int amount)
{
	if (hp_ == max_hp_) return 0;
	int new_hp_value = std::min(hp_ + amount, max_hp_);
	int amount_recovered = new_hp_value - hp_;
	hp_ = new_hp_value;
	return amount_recovered;
}

std::ostream& operator<<(std::ostream& os, const Fighter& fighter)
{
	os << fighter.max_hp_ << " " << fighter.hp_ << " " << fighter.defense_ << " " << fighter.power_ << "\n";
	return os;
}

std::istream& operator>>(std::istream& is, Fighter& fighter)
{
	is >> fighter.max_hp_ >> fighter.hp_ >> fighter.defense_ >> fighter.power_;
	return is;
}
