#include "input_handler.h"

#include <memory>

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

std::unique_ptr<Action> EventHandler::Dispatch() const
{
	SDL_Event event{};

	SDL_PollEvent(&event);
	std::unique_ptr<Action> game_event = nullptr;
	if (event.type == SDL_EVENT_KEY_DOWN)
		game_event = ev_keydown(event);

	return game_event;
}

std::unique_ptr<Action> EventHandler::ev_keydown(const SDL_Event& event) const
{
	std::unique_ptr<Action> action = nullptr;

	switch (event.key.key) {
	case SDLK_2:
	case SDLK_DOWN:
		action = std::make_unique<MovementAction>(0, 1);
		break;
	case SDLK_4:
	case SDLK_LEFT:
		action = std::make_unique<MovementAction>(-1, 0);
		break;
	case SDLK_6:
	case SDLK_RIGHT:
		action = std::make_unique<MovementAction>(1, 0);
		break;
	case SDLK_8:
	case SDLK_UP:
		action = std::make_unique<MovementAction>(0, -1);
		break;
	case SDLK_ESCAPE:
		action = std::make_unique<EscapeAction>();
		break;
	default:
		break;
	}

	return action;
}
