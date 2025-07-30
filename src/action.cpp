#include "action.h"
#include "entity.h"
#include "gamemap.h"
#include "engine.h"
#include "entity_manager.h"

#include <iostream>

void EscapeAction::Perform(Engine& engine, Entity& player) const {
	engine.Quit();
}

MovementAction::MovementAction(int dx, int dy) : ActionWithDirection(dx, dy){
}

ActionWithDirection::ActionWithDirection(int dx, int dy) : dx_(dx), dy_(dy)
{
}

MeleeAction::MeleeAction(int dx, int dy) : ActionWithDirection(dx, dy)
{
}

BumpAction::BumpAction(int dx, int dy) : ActionWithDirection(dx, dy)
{
}

void MovementAction::Perform(Engine& engine, Entity& player) const{
	int dest_x = player.GetX() + dx_;
	int dest_y = player.GetY() + dy_;
	if (!engine.GetMap().Inbound(dest_x, dest_y)) {
		return;
	}
	if (!engine.GetMap().GetTile(dest_x, dest_y).walkable) {
		return;
	}
	if (engine.GetEntities().GetBlockingEntity(dest_x, dest_y)) {
		return;
	}
	player.Move(dx_, dy_);
}

void MeleeAction::Perform(Engine& engine, Entity& player) const {
	int dest_x = player.GetX() + dx_;
	int dest_y = player.GetY() + dy_;
	Entity* target = engine.GetEntities().GetBlockingEntity(dest_x, dest_y);
	if (!target) return; // no entity
	std::cout << "you kick " << target->GetName() << " much to it's annoyance" << std::endl;
}


void BumpAction::Perform(Engine& engine, Entity& player) const {
	int dest_x = player.GetX() + dx_;
	int dest_y = player.GetY() + dy_;
	if (engine.GetEntities().GetBlockingEntity(dest_x, dest_y)) {
		return MeleeAction(dx_, dy_).Perform(engine, player);
	}
	return MovementAction(dx_, dy_).Perform(engine, player);
}
