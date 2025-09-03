#include "components/consumable.h"
#include "action.h"
#include "actor.h"
#include "engine.h"
#include "color.h"

#include <memory>
#include <format>


std::unique_ptr<Action> Consumable::GetAction(Actor& consumer) const
{
	return std::make_unique<ItemAction>(consumer, (Item&)*entity_);
}

bool HealingConsumable::Activate(Engine& engine, const ItemAction const* action) const
{
	Actor& consumer = action->GetActor();
	int amount_recovered = consumer.GetFighter().Heal(amount_);

	if (amount_recovered > 0) {
		engine.AddMessage(std::format("you consume the {}, and recovered {} HP!", entity_->GetName(), amount_recovered), health_recovered, true);
		return true;
	}
	else {
		engine.AddMessage("Your health is already full", impossible, true);
	}
	return false;
}

bool LightningDamageConsumable::Activate(Engine& engine, const ItemAction const* action) const {
	Actor& consumer = action->GetActor();
	Actor* target = nullptr;
	double closest_dist = max_range_ + 1;
	bool found = false;
	for (Actor& actor : engine.GetEntities()) {
		if (actor.IsDead()) continue;
		double distance = consumer.Distance(actor.GetX(), actor.GetY());
		if (distance < closest_dist && distance > 0) {
			found = true;
			target = &actor;
			closest_dist = distance;
		}
	}
	if (found) {
		engine.AddMessage(std::format("A lighting bolt strikes the {} with a loud thunder, for {} damage!", target->GetName(), damage_), player_atk, false);
		target->TakeDamage(damage_);
		return true;
	}
	else {
		engine.AddMessage(std::format("No enemy is close enough to strike."), white, false);
		return false;
	}
}