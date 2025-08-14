#include "action.h"
#include "actor.h"
#include "gamemap.h"
#include "engine.h"
#include "entity_manager.h"
#include "color.h"

#include <iostream>
#include <format>
#include <libtcod/color.hpp>

void EscapeAction::Perform(Engine& engine) const {
	engine.Quit();
}

MovementAction::MovementAction(Actor& entity, int dx, int dy) : ActionWithDirection(entity, dx, dy){
}

ActionWithDirection::ActionWithDirection(Actor& entity, int dx, int dy) : dx_(dx), dy_(dy), entity_(entity)
{
}

MeleeAction::MeleeAction(Actor& entity, int dx, int dy) : ActionWithDirection(entity, dx, dy)
{
}

BumpAction::BumpAction(Actor& entity, int dx, int dy) : ActionWithDirection(entity, dx, dy)
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
	Actor* target = engine.GetEntities().GetBlockingEntity(dest_x, dest_y);
	if (!target) return; // no entity
	int damage = entity_.GetPower() - target->GetDefense();
	std::string attack_desc = std::format("{} attacks {}", entity_.GetName(), target->GetName());
	tcod::ColorRGB atk_color;
	if (entity_.GetName() == engine.GetPlayer()->GetName()) atk_color = player_atk;
	else atk_color = enemy_atk;
	if (damage > 0) {
		target->TakeDamage(damage);
		engine.AddMessage(std::format("{} for {} hit point", attack_desc, damage), atk_color, true);
		if (!target->IsAlive() ){
			DieAction action = DieAction(*target);
			action.Perform(engine);
		}
	}
	else {
		engine.AddMessage(std::format("{} but does no damage", attack_desc), atk_color, true);
	}
}


void BumpAction::Perform(Engine& engine) const {
	int dest_x = entity_.GetX() + dx_;
	int dest_y = entity_.GetY() + dy_;
	if (engine.GetEntities().GetBlockingEntity(dest_x, dest_y)) {
		return MeleeAction(entity_, dx_, dy_).Perform(engine);
	}
	return MovementAction(entity_, dx_, dy_).Perform(engine);
}

void DieAction::Perform(Engine& engine) const {
	std::string death_message;
	tcod::ColorRGB death_color;
	if (entity_.GetName() == "player") {
		death_message = "You died!";
		death_color = player_die;
		engine.HandleDeath();
	}
	else {
		death_message = std::format("{} is dead!", entity_.GetName());
		death_color = enemy_die;
	}
	engine.AddMessage(death_message, death_color);
	entity_.Die();

}