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
#include "setup_game.h"

#include <iostream>
#include <fstream>

Engine::Engine(tcod::Context& context, tcod::Console& console, GameMap& map, GameWorld& gameworld) : entities_(EntityManager()), handler_(std::make_unique<MainMenu>(*this)), context_(std::make_unique<tcod::Context>(std::move(context.get_ptr()))), console_(console), map_(std::make_unique<GameMap>(map)), isrunning_(true), gameworld_(gameworld), player_(nullptr), messagelog_(MessageLog()), mouseloccation_({ 0,0 })
{
	gameworld_.GenerateFloor(*map_);
	entities_.Spawn(PLAYER, map_->GetRoom(0).Center());
	PlaceEntities();
	UpdateFov();
}

Engine::Engine(const Engine& engine) : entities_(EntityManager(engine.entities_)), handler_(engine.handler_->Clone()), context_(std::make_unique<tcod::Context>(std::move(engine.context_->get_ptr()))), console_(engine.console_), map_(std::make_unique<GameMap>(*engine.map_.get())), isrunning_(engine.isrunning_), gameworld_(engine.gameworld_), player_(nullptr), messagelog_(engine.messagelog_), mouseloccation_(engine.mouseloccation_)
{
	player_ = &entities_.Get(0);
	UpdateFov();
}


void Engine::HandleEvent()
{
	std::unique_ptr<Action> action = handler_->Dispatch();

	if (action) {
		action->Perform(*this);
		if (current_state_ == PLAYING) {
			HandleEnemyTurn();
			UpdateFov();
		}
	}

}

void Engine::Render()
{
	map_->Render(console_);
	std::vector<Actor> actors = entities_.Get();
	std::sort(actors.begin(), actors.end(), [](const Actor a, const Actor b) { return a.GetRendOrd() < b.GetRendOrd(); });
	for (Item& item : items_) {
		if (map_->IsInFov(item.GetX(), item.GetY())) {
			console_.at(item.GetX(), item.GetY()).ch = item.GetChar();
			console_.at(item.GetX(), item.GetY()).fg = item.GetColor();
		}
	}
	for (Actor& entity : actors) {
		if (map_->IsInFov(entity.GetX(), entity.GetY())){
			console_.at(entity.GetX(), entity.GetY()).ch = entity.GetChar();
			console_.at(entity.GetX(), entity.GetY()).fg = entity.GetColor();
		}
	}


	messagelog_.Render(console_, 32, 45, 40, 5, messagelog_.GetMessage());

	Renderer::RenderBar(console_, player_->GetHp(), player_->GetMaxHp(), 20, player_);
	Renderer::RenderNamesAtMouseLocation(*this, 21, 44);
	handler_->OnRender(console_);
	context_->present(console_);

	console_.clear();

}

void Engine::UpdateFov() const
{
	map_->UpdateFov(player_->GetX(), player_->GetY(), FOV_RADIUS);
}

void Engine::PlaceEntities()
{
	TCODRandom random;
	std::vector<RectangleRoom>& rooms = map_->GetRooms();
	for (RectangleRoom& room : rooms) {
		entities_.PlaceEntities(room, MAX_MONSTER_PER_ROOM, random);
		items_.PlaceEntities(room, MAX_ITEM_PER_ROOM, random);
	}
	for (Entity& entity : entities_) entity.SetMap(map_.get());
	for (Item& item : items_) item.SetMap(map_.get());
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
	handler_ = new_handler->Clone();
}

void Engine::LoadGame(std::string& filename)
{
	std::ifstream save_file(filename);
	if (!save_file.is_open()) {
		const std::unique_ptr<EventHandler> popup = std::make_unique<PopupMessage>(*this, "No game to load", handler_);
		SetEventHandler(popup);
		return;
	}
	save_file >> *this;
	for (auto& actor : entities_) {
		actor.SetMap(GetMap());
	}

	SetEventHandler(std::make_unique<MainGameEventHandler>(*this));
	current_state_ = PLAYING;
}

void Engine::SaveAs(const std::string& filename) const
{
	std::ofstream text_file(filename);
	text_file << *this;
}

std::ostream& operator<<(std::ostream& os, const Engine& engine) {
	os << *engine.map_ << std::endl;
	os << engine.entities_ << std::endl;
	os << engine.items_ << std::endl;
	os << engine.player_->GetInventory() << std::endl;
	return os;
}


std::istream& operator>>(std::istream& is, Engine& engine) {
	is >> *engine.map_;
	is >> engine.entities_;

	engine.SetPlayer();


	is >> engine.items_;
	is >> engine.player_->GetInventory();
	return is;
}