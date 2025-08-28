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

void HealingConsumable::Activate(Engine& engine, const ItemAction const* action) const
{
	Actor& consumer = action->GetActor();
	int amount_recovered = consumer.GetFighter().Heal(amount_);

	if (amount_recovered > 0) {
		engine.AddMessage(std::format("you consume the {}, and recovered {} HP!", entity_->GetName(), amount_recovered), health_recovered, true);
	}
	else {
		engine.AddMessage("Your health is already full", impossible, true);

	}
}
