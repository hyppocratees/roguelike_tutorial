#pragma once

#include <libtcod/libtcod.hpp>
#include <vector>
#include <utility>
#include <memory>

#include "input_handler.h"
#include "actor.h"
#include "gamemap.h"
#include "entity_manager.h"
#include "item_manager.h"
#include "procgen.h"
#include "message_log.h"

enum GameState {
	PLAYING,
	PAUSED
};

class Engine {
public:

	Engine(tcod::Context& context, tcod::Console& console, GameMap& map, MapGenerator& mapgen);
	Engine(const Engine& engine);

	void HandleEvent();
	void Render();

	GameMap* GetMap() const { return map_.get(); };
	EntityManager& GetEntities() { return entities_; };
	const EntityManager& GetEntities() const { return entities_; };
	ItemManager& GetItem() { return items_; };
	const ItemManager& GetItem() const { return items_; };
	tcod::Context* GetContext() const { return context_.get(); };
	tcod::Console& GetConsole() const { return console_; };

	bool IsRunning() const { return isrunning_; };
	void Quit() { isrunning_ = false; };

	void UpdateFov() const;
	void PlaceEntities();
	void HandleEnemyTurn();
	void HandleDeath();
	Actor* GetPlayer() { return player_; };
	std::pair<int, int> GetMouseLocation() const { return mouseloccation_; };
	std::pair<int, int>& GetMouseLocation() { return mouseloccation_; };
	void SetMouseLocation(int x, int y) { mouseloccation_ = { x, y }; };
	void SetMouseLocation(std::pair<int, int> loc) { mouseloccation_ = loc; };

	void AddMessage(std::string text, tcod::ColorRGB color, bool stack = false) { messagelog_.AddMessage(text, color, stack); };
	MessageLog& GetMessage() { return messagelog_; };

	void SetEventHandler(const std::unique_ptr<EventHandler>& new_handler);

	void LoadGame(std::string& filename);
	void SaveAs(const std::string& filename) const;    
	void SetPlayer() { player_ = &entities_.GetPlayer();};
	void SetGameState(GameState new_state_) { current_state_ = new_state_; }

	friend std::ostream& operator<<(std::ostream& os, const Engine& engine);
	friend std::istream& operator>>(std::istream& is, Engine& engine);

private:
	std::unique_ptr<tcod::Context> context_;
	tcod::Console& console_;
	EntityManager entities_;
	ItemManager items_;
	std::unique_ptr<EventHandler> handler_;
	Actor* player_;
	std::unique_ptr<GameMap> map_;
	MapGenerator& mapgen_;
	MessageLog messagelog_;
	std::pair<int, int> mouseloccation_;
	bool isrunning_;
	GameState current_state_ = PAUSED;
};