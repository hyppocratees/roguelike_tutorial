#pragma once
#include <libtcod.hpp>
#include <libtcod/image.hpp>
#include "input_handler.h"

class Engine;

Engine NewGame(tcod::Console& console, TCOD_ContextParams& params);

class MainMenu : public EventHandler {
public:
	MainMenu(Engine& engine) : EventHandler(engine) {
		background_ = TCODImage("../menu_background.png");
	};
	MainMenu(const MainMenu& menu) : EventHandler(engine_), background_(TCODImage("../menu_background.png")){}
	virtual void OnRender(tcod::Console& console);
	virtual std::unique_ptr<EventHandler> Clone() const;
protected:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
private:
	TCODImage background_;
};