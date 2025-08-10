#include "input_handler.h"
#include "engine.h"

#include <memory>

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

std::unique_ptr<Action> EventHandler::Dispatch() const
{
	SDL_Event event{};

	SDL_PollEvent(&event);
	std::unique_ptr<Action> game_event = nullptr;
	if (event.type == SDL_EVENT_KEY_DOWN)
		game_event = EvKeydown(event);
	if (event.type == SDL_EVENT_QUIT) {
		game_event = std::make_unique<EscapeAction>();
	}

	return game_event;
}

std::unique_ptr<Action> MainGameEventHandler::EvKeydown(const SDL_Event& event) const
{
	std::unique_ptr<Action> action = nullptr;

	switch (event.key.key) {
	case SDLK_1:
	case SDLK_KP_1:		
		action = std::make_unique<BumpAction>(*engine_.GetPlayer(), -1, 1);
		break;
	case SDLK_2:
	case SDLK_KP_2:
	case SDLK_DOWN:
		action = std::make_unique<BumpAction>(*engine_.GetPlayer(), 0, 1);
		break;
	case SDLK_3:
	case SDLK_KP_3:
		action = std::make_unique<BumpAction>(*engine_.GetPlayer(), 1, 1);
		break;
	case SDLK_4:
	case SDLK_KP_4:
	case SDLK_LEFT:
		action = std::make_unique<BumpAction>(*engine_.GetPlayer(), -1, 0);
		break;
	case SDLK_6:
	case SDLK_KP_6:
	case SDLK_RIGHT:
		action = std::make_unique<BumpAction>(*engine_.GetPlayer(), 1, 0);
		break;
	case SDLK_7:
	case SDLK_KP_7:
		action = std::make_unique<BumpAction>(*engine_.GetPlayer(), -1, -1);
		break;
	case SDLK_8:
	case SDLK_KP_8:
	case SDLK_UP:
		action = std::make_unique<BumpAction>(*engine_.GetPlayer(), 0, -1);
		break;
	case SDLK_9:
	case SDLK_KP_9:
		action = std::make_unique<BumpAction>(*engine_.GetPlayer(), 1, -1);
		break;
	case SDLK_5:
	case SDLK_KP_5:
		action = std::make_unique<WaitAction>(*engine_.GetPlayer());
		break;
	case SDLK_ESCAPE:
		action = std::make_unique<EscapeAction>();
		break;
	default:
		break;
	}

	return action;
}

std::unique_ptr<Action> GameOverEventHandler::EvKeydown(const SDL_Event& event) const {
	std::unique_ptr<Action> action = nullptr;

	switch (event.key.key) {
	case SDLK_ESCAPE:
		action = std::make_unique<EscapeAction>();
		break;
	default:
		break;
	}
	return action;
}