#include "engine.h"
#include "input_handler.h"
#include "gamemap.h"
#include "glob_var.h"
#include "entity_manager.h"
#include "procgen.h"
#include "entity_factory.h"
#include "renderer.h"
#include "message_log.h"

#include <libtcod/mersenne.hpp>
#include <algorithm>
#include "actor.h"

#include <iostream>

Engine::Engine(tcod::Context& context, tcod::Console& console, GameMap& map, MapGenerator& mapgen) : entities_(EntityManager()), handler_(std::make_unique<MainGameEventHandler>(*this)), context_(context), console_(console), map_(map), isrunning_(true), mapgen_(mapgen), player_(nullptr), messagelog_(MessageLog()), mouseloccation_({0,0})
{
	mapgen_.Generate(map_);
	entities_.Spawn(PLAYER, map_.GetRoom(0).Center());
	PlaceEntities();
	UpdateFov();
}

void Engine::HandleEvent()
{
	std::unique_ptr<Action> action = handler_->Dispatch();

	if (action) {
		action->Perform(*this);
		HandleEnemyTurn();
		UpdateFov();
	}

}

void Engine::Render()
{
	map_.Render(console_);
	std::vector<Actor> actors = entities_.Get();
	std::sort(actors.begin(), actors.end(), [](const Actor a, const Actor b) { return a.GetRendOrd() < b.GetRendOrd(); });
	for (Item& item : items_) {
		if (map_.IsInFov(item.GetX(), item.GetY())) {
			console_.at(item.GetX(), item.GetY()).ch = item.GetChar();
			console_.at(item.GetX(), item.GetY()).fg = item.GetColor();
		}
	}
	for (Actor& entity : actors) {
		if (map_.IsInFov(entity.GetX(), entity.GetY())){
			console_.at(entity.GetX(), entity.GetY()).ch = entity.GetChar();
			console_.at(entity.GetX(), entity.GetY()).fg = entity.GetColor();
		}
	}


	messagelog_.Render(console_, 32, 45, 40, 5, messagelog_.GetMessage());

	Renderer::RenderBar(console_, player_->GetHp(), player_->GetMaxHp(), 20, player_);
	Renderer::RenderNamesAtMouseLocation(*this, 21, 44);
	handler_->OnRender(console_);
	context_.present(console_);

	console_.clear();

}

void Engine::UpdateFov() const
{
	map_.UpdateFov(player_->GetX(), player_->GetY(), FOV_RADIUS);
}

void Engine::PlaceEntities()
{
	TCODRandom random;
	std::vector<RectangleRoom>& rooms = map_.GetRooms();
	for (RectangleRoom& room : rooms) {
		entities_.PlaceEntities(room, MAX_MONSTER_PER_ROOM, random);
		items_.PlaceEntities(room, MAX_ITEM_PER_ROOM, random);
	}
	for (Entity& entity : entities_) entity.SetMap(&map_);
	for (Item& item : items_) item.SetMap(&map_);
	player_ = &entities_.Get(0);
}

void Engine::HandleEnemyTurn()
{
	for (auto& entity : entities_) {
		if (entity.GetName() == "player" || !entity.IsAlive()) continue;
		entity.GetAI()->Perform(*this);
		//std::cout << "The " << entity.GetName() << " wonders when it will get to take a real turn." << std::endl;
	}
}

void Engine::HandleDeath()
{
	handler_ = std::make_unique<GameOverEventHandler>(*this);
}

void Engine::SetEventHandler(const std::unique_ptr<EventHandler>& new_handler)
{
	handler_ = new_handler->clone();
}
