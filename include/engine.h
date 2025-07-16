#pragma once

#include <libtcod/libtcod.hpp>

#include "input_handler.h"
#include "entity.h"
#include <vector>
#include <memory>

class Engine {
public:

	Engine(std::vector<Entity>& entities, EventHandler handler, Entity& player, tcod::Context& context, tcod::Console& console);

	void HandleEvent();
	void Render();

private:
	tcod::Context& context_;
	tcod::Console& console_;
	std::vector<Entity>& entities_;
	EventHandler handler_;
	Entity& player_;
};