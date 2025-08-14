#pragma once

#include <libtcod/libtcod.hpp>
#include <vector>
#include <memory>

#include "input_handler.h"
#include "actor.h"
#include "gamemap.h"
#include "entity_manager.h"
#include "procgen.h"
#include "message_log.h"


class Engine {
public:

	Engine(tcod::Context& context, tcod::Console& console, GameMap& map, MapGenerator& mapgen);

	void HandleEvent();
	void Render();

	GameMap& GetMap() const { return map_; };
	EntityManager& GetEntities() { return entities_; };

	bool IsRunning() const { return isrunning_; };
	void Quit() { isrunning_ = false; };

	void UpdateFov() const;
	void PlaceEntities();
	void HandleEnemyTurn();
	void HandleDeath();
	Actor* GetPlayer() { return player_; };

	void AddMessage(std::string text, tcod::ColorRGB color) { messagelog_.AddMessage(text, color); };

private:
	tcod::Context& context_;
	tcod::Console& console_;
	EntityManager entities_;
	std::unique_ptr<EventHandler> handler_;
	Actor* player_;
	GameMap& map_;
	MapGenerator& mapgen_;
	MessageLog messagelog_;
	bool isrunning_;
};