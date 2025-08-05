#pragma once
#include <memory>
#include <SDL3/SDL.h>

#include "action.h"

class Engine;

class EventHandler {
public:
	EventHandler(Engine& engine) : engine_(engine) {}
	std::unique_ptr<Action> Dispatch() const;
private:
	std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
	Engine& engine_;
};