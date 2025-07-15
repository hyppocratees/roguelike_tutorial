#pragma once
#include <memory>
#include <SDL3/SDL.h>

#include "action.h"

class EventHandler {
public:
	std::unique_ptr<Action> Dispatch() const;
private:
	std::unique_ptr<Action> ev_keydown(const SDL_Event& event) const;
};