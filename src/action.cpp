#include "action.h"
#include "entity.h"
#include "gamemap.h"
#include "engine.h"
#include "entity_manager.h"

#include <iostream>

void EscapeAction::Perform(Engine& engine) const {
	engine.Quit();
}

MovementAction::MovementAction(Entity& entity, int dx, int dy) : ActionWithDirection(entity, dx, dy){
}

ActionWithDirection::ActionWithDirection(Entity& entity, int dx, int dy) : dx_(dx), dy_(dy), entity_(entity)
{
}

MeleeAction::MeleeAction(Entity& entity, int dx, int dy) : ActionWithDirection(entity, dx, dy)
{
}

BumpAction::BumpAction(Entity& entity, int dx, int dy) : ActionWithDirection(entity, dx, dy)
{
}

void MovementAction::Perform(Engine& engine) const{
	int dest_x = entity_.GetX() + dx_;
	int dest_y = entity_.GetY() + dy_;
	if (!engine.GetMap().Inbound(dest_x, dest_y)) {
		return;
	}
	if (!engine.GetMap().GetTile(dest_x, dest_y).walkable) {
		return;
	}
	if (engine.GetEntities().GetBlockingEntity(dest_x, dest_y)) {
		return;
	}
	entity_.Move(dx_, dy_);
}

void MeleeAction::Perform(Engine& engine) const {
	int dest_x = entity_.GetX() + dx_;
	int dest_y = entity_.GetY() + dy_;
	Entity* target = engine.GetEntities().GetBlockingEntity(dest_x, dest_y);
	if (!target) return; // no entity
	std::cout << "you kick " << target->GetName() << " much to it's annoyance" << std::endl;
}


void BumpAction::Perform(Engine& engine) const {
	int dest_x = entity_.GetX() + dx_;
	int dest_y = entity_.GetY() + dy_;
	if (engine.GetEntities().GetBlockingEntity(dest_x, dest_y)) {
		return MeleeAction(entity_, dx_, dy_).Perform(engine);
	}
	return MovementAction(entity_, dx_, dy_).Perform(engine);
}
