#include "components/consumable.h"
#include "action.h"
#include "actor.h"
#include "engine.h"
#include "color.h"
#include "item.h"
#include "components/ai.h"

#include <memory>
#include <format>
#include <utility>


std::unique_ptr<Action> Consumable::GetAction(Actor& consumer) const
{
	return std::make_unique<ItemAction>(consumer, (Item&)*entity_);
}

std::unique_ptr<Action> Consumable::GetAction(Engine& engine, Actor& consumer, Item& item) const
{
	return GetAction(consumer);
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

std::unique_ptr<Action> ConfusionConsumable::GetAction(Engine& engine, Actor& consumer, Item& item) const {
	engine.AddMessage("Select a target location", need_target, false);
	engine.SetEventHandler(std::make_unique<SingleRangedAttackHandler>(engine, [&](int x, int y) {ItemAction(consumer, item, { x, y }).Perform(engine); }));
	return std::make_unique<ItemAction>(consumer, (Item&)*entity_);
}

bool ConfusionConsumable::Activate(Engine& engine, const ItemAction const* action) const {
	if (!action) return false;
	auto[x, y] = action->GetPos();
	if (!engine.GetMap()->IsInFov(x, y)) {
		engine.AddMessage("You cannot target an area that you cannot see.", white, false);
		return false;
	}
	Actor* target = engine.GetEntities().GetBlockingEntity(x, y);
	if (!target) {
		engine.AddMessage("You must target an enemy.", white, false);
		return false;
	}
	if (target == engine.GetPlayer()) {
		engine.AddMessage("You cannot target yourself.", white, false);
		return false;
	}
	engine.AddMessage(std::format("The eyes of the {} look vacant, as it starts to stumble around!", target->GetName()), status_effect_applied, false);

	target->SetAI(std::make_unique<ConfusedAI>(target, target->GetAI(), 10));
	engine.SetEventHandler(std::make_unique<MainGameEventHandler>(engine));
	return true;
}

bool FireballConsumable::Activate(Engine& engine, const ItemAction const* action) const {
	if (!action) return false;
	auto[x, y] = action->GetPos();
	if (!engine.GetMap()->IsInFov(x, y)) {
		engine.AddMessage("You cannot target an area that you cannot see.", white, false);
		return false;
	}
	bool target_hit = false;
	for (Actor& actor : engine.GetEntities()) {
		if (actor.Distance(x, y) <= radius_) {
			engine.AddMessage(std::format("The {}, is engulfed in a fiery explosion, taking {} damage!", actor.GetName(), damage_), white, true);
			actor.TakeDamage(damage_);
			target_hit = true;
		}
	}
	if (!target_hit) {
		engine.AddMessage("There are no targets in the radius.", white, true);
	}
	engine.SetEventHandler(std::make_unique<MainGameEventHandler>(engine));
	return true;
}

std::unique_ptr<Action> FireballConsumable::GetAction(Engine& engine, Actor& consumer, Item& item) const {
	engine.AddMessage("Select a target location", need_target, false);
	engine.SetEventHandler(std::make_unique<AreaRangedAttackHandler>(engine, radius_, [&](int x, int y) {ItemAction(consumer, item, { x, y }).Perform(engine); }));
	return std::make_unique<ItemAction>(consumer, (Item&)*entity_);
}

void Consumable::ToText(std::ostream& os) const {
	os << 0 << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Consumable& cons) {
	cons.ToText(os);
	return os;
}

void HealingConsumable::ToText(std::ostream& os) const {
	os << 1 << std::endl;
	os << amount_ << "\n";
}

void FireballConsumable::ToText(std::ostream& os) const {
	os << 2 << std::endl;
	os << damage_ << " " << radius_ << "\n";
}

void LightningDamageConsumable::ToText(std::ostream& os) const {
	os << 3 << std::endl;
	os << damage_ << " " << max_range_ << "\n";
}

void ConfusionConsumable::ToText(std::ostream& os) const {
	os << 4 << std::endl;
	os << number_turn_ << "\n";
}