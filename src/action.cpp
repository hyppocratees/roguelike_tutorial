#include "action.h"
#include "actor.h"
#include "gamemap.h"
#include "engine.h"
#include "entity_manager.h"
#include "color.h"
#include "inventory.h"
#include "item.h"
#include "input_handler.h"
#include "handler_factory.h"

#include <iostream>
#include <format>
#include <libtcod/color.hpp>
#include <memory>
#include <filesystem>

void EscapeAction::Perform(Engine& engine) const {
	if (std::filesystem::exists("../savegame.sav")) {
		std::filesystem::remove("../savegame.sav");
	}
	if (engine.GetPlayer()->IsAlive()) {
		engine.SaveAs("../savegame.sav");
	}
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
	if (!engine.GetMap()->Inbound(dest_x, dest_y)) {
		return;
	}
	if (!engine.GetMap()->GetTile(dest_x, dest_y).walkable) {
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
	if (entity_.GetName().find("player") != std::string::npos) {
		death_message = "You died!";
		death_color = player_die;
		engine.HandleDeath();
	}
	else {
		death_message = std::format("{} is dead!", entity_.GetName());
		death_color = enemy_die;
	}
	engine.AddMessage(death_message, death_color);
	engine.GetPlayer()->GetLevel().GainXp(engine, entity_.GetLevel().GetXp());
	//entity_.Die();

}

void ReturnToMainGame::Perform(Engine& engine) const {
	engine.SetEventHandler(std::make_unique<MainGameEventHandler>(engine));
	engine.SetGameState(PLAYING);
}

void ItemAction::Perform(Engine& engine) const {
	if (item_.GetConsumable()) {
		if( item_.GetConsumable()->Activate(engine, this)){
			actor_.GetInventory().Remove(item_);
		}
	}
}

void PickupAction::Perform(Engine& engine) const {
	Inventory& inv = actor_.GetInventory();
	ItemManager& itemman = engine.GetItem();
	for (Item& item : itemman) {
		if (item.GetPos() == actor_.GetPos()) {
			if (inv.Size() >= inv.Capacity()) {
				engine.AddMessage("Your Inventory is full", impossible, true);
				return;
			}
			inv.Add(item);
			engine.AddMessage(std::format("You picked up the {}", item.GetName()), white, true);
			itemman.Remove(item);
			return;
		}
	}
	engine.AddMessage("There is nothing here to pick up", error, true);
}

void DropAction::Perform(Engine& engine) const {
	engine.GetItem().Spawn(item_, actor_.GetPos());
	actor_.GetInventory().Drop(item_);
}

void LoadAction::Perform(Engine& engine) const {
	std::string savename = "../savegame.sav";
	engine.LoadGame(savename);
}

SetHandlerAction::SetHandlerAction(std::unique_ptr<EventHandler> handler) : handler_(handler->Clone()) {}

void SetHandlerAction::Perform(Engine& engine) const {
	engine.SetEventHandler(HandlerFactory::GetHandler(engine, handler_->Type()));
}

void TakeDownStairAction::Perform(Engine& engine) const {
	if (engine.GetPlayer()->GetPos() == engine.GetMap()->GetDownStairLocation()) {
		engine.GetGameWorld().GenerateFloor(*engine.GetMap());
		engine.GetEntities().ClearNPC();
		engine.GetItem().Clear();
		engine.PlaceEntities();
		engine.AddMessage("You descend the staircase", descend);
		engine.GetPlayer()->SetX(engine.GetMap()->GetRoom(0).Center().first);
		engine.GetPlayer()->SetY(engine.GetMap()->GetRoom(0).Center().second);
	}
	else {
		engine.AddMessage("There is no stairs here", invalid);
	}
}

void GenerateLevel::Perform(Engine& engine) const {
	engine.GetGameWorld().GenerateFloor(*engine.GetMap());
}