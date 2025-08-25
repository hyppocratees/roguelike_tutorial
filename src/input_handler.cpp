#include "input_handler.h"
#include "engine.h"
#include "message_log.h"

#include <memory>
#include <algorithm>
#include <libtcod/libtcod.hpp>

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

std::unique_ptr<Action> EventHandler::Dispatch() const
{
	SDL_Event event{};

	SDL_PollEvent(&event);
	engine_.GetContext().convert_event_coordinates(event);
	std::unique_ptr<Action> game_event = nullptr;
	if (event.type == SDL_EVENT_KEY_DOWN)
		game_event = EvKeydown(event);
	if (event.type == SDL_EVENT_QUIT) {
		game_event = std::make_unique<EscapeAction>();
	}
	if (event.type == SDL_EVENT_MOUSE_MOTION) {
		EvMouseMotion(event);
	}

	return game_event;
}

std::unique_ptr<EventHandler> MainGameEventHandler::clone() const
{
	return std::make_unique<MainGameEventHandler>(*this);
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
	case SDLK_V:
		engine_.SetEventHandler(std::make_unique<HistoryViewerHandler>(engine_));
		break;
	default:
		break;
	}

	return action;
}

void EventHandler::EvMouseMotion(const SDL_Event& event) const
{
	if (engine_.GetMap().Inbound(event.motion.x, event.motion.y)){
		engine_.SetMouseLocation(event.motion.x, event.motion.y);
	}
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

HistoryViewerHandler::HistoryViewerHandler(Engine& engine) : EventHandler(engine), log_length_(engine.GetMessage().GetMessage().size()), cursor_(log_length_ - 1) {};

void HistoryViewerHandler::OnRender(tcod::Console& cons) {
	tcod::Console log_console{ cons.get_width() - 6, cons.get_height() - 6 };
	tcod::draw_frame(log_console, { 0, 0, log_console.get_width(), log_console.get_height() }, { 0, 1, 2, 3, 4, 5, 6, 7, 8 }, { {255, 255, 255} }, { {0, 0, 0} });
	tcod::print_rect(log_console, { 0, 0, log_console.get_width(), 1 }, "┤Message history├", { {255, 255, 255} }, { {0, 0, 0} }, TCOD_CENTER);
	std::vector<Message> message = engine_.GetMessage().GetMessage();
	std::vector<Message> to_render = std::vector<Message>(message.begin() + cursor_, message.end());
	engine_.GetMessage().Render(log_console, 1, 1, log_console.get_width() - 2, log_console.get_height() - 2, to_render);
	tcod::blit(cons, log_console, { 3, 3 });
}

std::unique_ptr<Action> HistoryViewerHandler::EvKeydown(const SDL_Event& event) const {
	int adjust = 0;
	std::unique_ptr<Action> action = nullptr;

	switch (event.key.key) {
	case SDLK_UP:
		adjust = -1;
		break;
	case SDLK_DOWN:
		adjust = 1;
		break;
	case SDLK_PAGEUP:
		adjust = -10;
		break;
	case SDLK_PAGEDOWN:
		adjust = 10;
		break;
	case SDLK_HOME:
		cursor_ = 0;
		break;
	case SDLK_END:
		cursor_ = log_length_ - 1;
		break;
	default:
		action = std::make_unique<ReturnToMainGame>();
	}

	if (adjust < 0 && cursor_ == 0) {
		cursor_ = log_length_ - 1;
	}
	else if (adjust > 0 && cursor_ == log_length_ - 1) {
		cursor_ = 0;
	}
	else if (adjust != 0) {
		cursor_ = std::max(0, std::min(cursor_ + adjust, log_length_ - 1));
	}

	return action;
}

std::unique_ptr<EventHandler> HistoryViewerHandler::clone() const {
	return std::make_unique<HistoryViewerHandler>(*this);
}

std::unique_ptr<EventHandler> GameOverEventHandler::clone() const {
	return std::make_unique<GameOverEventHandler>(*this);
}