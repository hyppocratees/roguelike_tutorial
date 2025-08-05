#include "engine.h"
#include "input_handler.h"
#include "gamemap.h"
#include "glob_var.h"
#include "entity_manager.h"
#include "procgen.h"
#include "entity_factory.h"
#include <libtcod/mersenne.hpp>

#include <iostream>

Engine::Engine(tcod::Context& context, tcod::Console& console, GameMap& map, MapGenerator& mapgen) : entities_(EntityManager()), handler_(EventHandler(*this)), context_(context), console_(console), map_(map), isrunning_(true), mapgen_(mapgen), player_(nullptr)
{
	mapgen_.Generate(map_);
	entities_.Spawn(PLAYER, map_.GetRoom(0).Center());
	PlaceEntities();
	UpdateFov();
}

void Engine::HandleEvent()
{
	std::unique_ptr<Action> action = handler_.Dispatch();

	if (action) {
		action->Perform(*this);
		HandleEnemyTurn();
		UpdateFov();
	}

}

void Engine::Render()
{
	map_.Render(console_);

	for (Entity& entity : entities_) {
		if (map_.IsInFov(entity.GetX(), entity.GetY())){
			console_.at(entity.GetX(), entity.GetY()).ch = entity.GetChar();
			console_.at(entity.GetX(), entity.GetY()).fg = entity.GetColor();
		}
	}

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
	}
	player_ = &entities_.Get(0);
}

void Engine::HandleEnemyTurn()
{
	for (auto& entity : entities_) {
		std::cout << "The " << entity.GetName() << " wonders when it will get to take a real turn." << std::endl;
	}
}
