#pragma once

#include <libtcod/libtcod.hpp>

#include "input_handler.h"
#include "entity.h"
#include "gamemap.h"
#include <vector>



class Engine {
public:

	Engine(std::vector<Entity>& entities, Entity& player, tcod::Context& context, tcod::Console& console, GameMap& map);

	void HandleEvent();
	void Render();

	GameMap& GetMap() const { return map_; };

	bool IsRunning() const { return isrunning_; };
	void Quit() { isrunning_ = false; };

	void UpdateFov() const;

private:
	tcod::Context& context_;
	tcod::Console& console_;
	std::vector<Entity>& entities_;
	EventHandler handler_;
	Entity& player_;
	GameMap& map_;
	bool isrunning_;
};