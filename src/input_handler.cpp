#include "input_handler.h"
#include "engine.h"
#include "message_log.h"

#include <memory>
#include <map>
#include <algorithm>
#include <libtcod/libtcod.hpp>
#include <string>

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

std::map<SDL_Keycode, std::pair<int, int>> MoveKey{ {SDLK_1, {-1, 1}}, {SDLK_2, {0, 1} }, {SDLK_3, {1, 1}}, {SDLK_4, {-1, 0}}, {SDLK_5, {0, 0}}, {SDLK_6, {1, 0} }, {SDLK_7, {-1, -1} }, {SDLK_8, {0, -1}}, {SDLK_9, {1, -1}},
	{SDLK_UP, {0, -1}}, {SDLK_DOWN, {0, 1}}, {SDLK_RIGHT, {1, 0}}, {SDLK_LEFT, {-1, 0}},
	{SDLK_KP_1, {-1, 1}}, {SDLK_KP_2, {0, 1} }, {SDLK_KP_3, {1, 1}}, {SDLK_KP_4, {-1, 0}}, {SDLK_KP_5, {0, 0}}, {SDLK_KP_6, {1, 0} }, {SDLK_KP_7, {-1, -1} }, {SDLK_KP_8, {0, -1}}, {SDLK_KP_9, {1, -1}} };


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

std::unique_ptr<EventHandler> MainGameEventHandler::Clone() const
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
	case SDLK_P:
		action = std::make_unique<PickupAction>(*engine_.GetPlayer());
		break;
	case SDLK_I:
		engine_.SetEventHandler(std::make_unique<InventoryActivateHandler>(engine_));
		break;
	case SDLK_D:
		engine_.SetEventHandler(std::make_unique<InventoryDropHandler>(engine_));
		break;
	case SDLK_L:
		engine_.SetEventHandler(std::make_unique<LookHandler>(engine_));
		break;
	default:
		break;
	}

	return action;
}

std::unique_ptr<Action> EventHandler::EvMouseMotion(const SDL_Event& event) const
{
	if (engine_.GetMap().Inbound(event.motion.x, event.motion.y)){
		engine_.SetMouseLocation(event.motion.x, event.motion.y);
	}
	return nullptr;
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

std::unique_ptr<EventHandler> HistoryViewerHandler::Clone() const {
	return std::make_unique<HistoryViewerHandler>(*this);
}

std::unique_ptr<EventHandler> GameOverEventHandler::Clone() const {
	return std::make_unique<GameOverEventHandler>(*this);
}

AskUserEventHandler::AskUserEventHandler(Engine& engine) : EventHandler(engine)
{
}

std::unique_ptr<EventHandler> AskUserEventHandler::Clone() const {
	return std::make_unique<AskUserEventHandler>(*this);
}

std::unique_ptr<Action> AskUserEventHandler::EvKeydown(const SDL_Event& event) const {
	std::unique_ptr<Action> action = nullptr;

	action = std::make_unique<ReturnToMainGame>();
	
	return action;
}

InventoryEventHandler::InventoryEventHandler(Engine& engine) : AskUserEventHandler(engine)
{
}

InventoryEventHandler::InventoryEventHandler(Engine& engine, std::string title) : AskUserEventHandler(engine), title_(title)
{
}

std::unique_ptr<EventHandler> InventoryEventHandler::Clone() const {
	return std::make_unique<InventoryEventHandler>(*this);
}

void InventoryEventHandler::OnRender(tcod::Console& cons) {
	Inventory& inv = engine_.GetPlayer()->GetInventory();
	int number_of_item = inv.Size();
	int height = number_of_item + 2;
	if (height <= 3) height = 3;
	int x = 0;
	if (engine_.GetPlayer()->GetX() <= 30) x = 40;
	else x = 0;
	int y = 0;
	int width = title_.size() + 4;
	tcod::Console inv_cons{width, height};
	tcod::draw_frame(inv_cons, { 0, 0, width, height }, { 0, 1, 2, 3, 4, 5, 6, 7, 8 }, { {255, 255, 255} }, { {0, 0, 0} });
	tcod::print_rect(inv_cons, { 2, 0, width, height }, title_, { {255, 255, 255} }, { {0, 0, 0} }, TCOD_CENTER);
	if (number_of_item > 0) {
		for (int i = 0; i < number_of_item; ++i) {
			char item_letter = (char)((int)'a' + i);
			tcod::print(inv_cons, { 1, i + 1 }, std::format("{}) {}", item_letter, inv[i].GetName()), { {255, 255, 255} }, { {0, 0, 0} }, TCOD_LEFT, TCOD_BKGND_SET);
		}
	}
	else tcod::print(inv_cons, { 1, 1 }, "(Empty)", {{255, 255, 255}}, {{0, 0, 0}}, TCOD_LEFT, TCOD_BKGND_SET);
	tcod::blit(cons, inv_cons, { x, y });
}

std::unique_ptr<Action> InventoryEventHandler::EvKeydown(const SDL_Event& event) const {
	
	
	SDL_Keycode sym = event.key.key;

	if (sym == SDLK_ESCAPE) return std::make_unique<ReturnToMainGame>();
	if (SDLK_A <= sym && sym <= SDLK_Z) {
		try {
			Item& item = engine_.GetPlayer()->GetInventory()[sym - SDLK_A];
			Dispatch();
			return OnItemSelected(&item);
		}
		catch (...){
			engine_.AddMessage("Invalid Entry", invalid, true);
		}
	}

	return nullptr;
}

std::unique_ptr<Action> InventoryEventHandler::OnItemSelected(Item* item) const {
	return std::make_unique<ItemAction>(*engine_.GetPlayer(), *item );
}

InventoryActivateHandler::InventoryActivateHandler(Engine& engine) : InventoryEventHandler(engine, "Select an item to use") {
}

std::unique_ptr<EventHandler> InventoryActivateHandler::Clone() const {
	return std::make_unique<InventoryActivateHandler>(*this);
}

std::unique_ptr<Action> InventoryActivateHandler::OnItemSelected(Item* item) const {
	if (item->GetConsumable()) {
		return item->GetConsumable()->GetAction(*engine_.GetPlayer());
	}
	return nullptr;
}

InventoryDropHandler::InventoryDropHandler(Engine& engine) : InventoryEventHandler(engine, "Select an item to drop")
{
}

std::unique_ptr<EventHandler> InventoryDropHandler::Clone() const {
	return std::make_unique<InventoryDropHandler>(*this);
}

std::unique_ptr<Action> InventoryDropHandler::OnItemSelected(Item* item) const {
	return std::make_unique<DropAction>(*engine_.GetPlayer(), *item);
}

SelectIndexHandler::SelectIndexHandler(Engine& engine) : AskUserEventHandler(engine), mouse_location_(engine.GetMouseLocation())
{
	engine_.SetMouseLocation(engine_.GetPlayer()->GetPos());
}

void SelectIndexHandler::OnRender(tcod::Console& console) {
	console.at(mouse_location_.first, mouse_location_.second).bg = white;
	console.at(mouse_location_.first, mouse_location_.second).fg = black;
}

std::unique_ptr<Action> SelectIndexHandler::EvKeydown(const SDL_Event& event) const {
	std::unique_ptr<Action> action = nullptr;
	int modifier = 1;
	auto lambda = [&](auto it) {return it.first == event.key.key; };
	auto found = std::find_if(MoveKey.begin(), MoveKey.end(), lambda);
	if (found != MoveKey.end()) {
		if (event.key.mod & SDL_KMOD_SHIFT)
			modifier *= 5;
		else if (event.key.mod & SDL_KMOD_CTRL)
			modifier *= 10;
		else if (event.key.mod & SDL_KMOD_ALT)
			modifier *= 20;
		int x = engine_.GetMouseLocation().first;
		int y = engine_.GetMouseLocation().second;
		std::pair<int, int> dxdy = MoveKey[event.key.key];
		x += dxdy.first * modifier;
		y += dxdy.second * modifier;
		x = std::max(0, std::min(x, engine_.GetMap().GetWidth() - 1));
		y = std::max(0, std::min(y, engine_.GetMap().GetHeight() - 1));
		engine_.SetMouseLocation({ x, y });
		return nullptr;
	}
	else if (event.key.key == SDLK_RETURN || event.key.key == SDLK_KP_ENTER || event.key.key == SDLK_CLEAR || event.key.key == SDLK_ESCAPE) {
		return OnIndexSelected(engine_.GetMouseLocation());
	}
}

std::unique_ptr<Action> SelectIndexHandler::EvMouseMotion(const SDL_Event& event) const {
	if (engine_.GetMap().Inbound(event.motion.x, event.motion.y)) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			return OnIndexSelected(event.motion.x, event.motion.y);
		}
	}
	return nullptr;
}

std::unique_ptr<Action> LookHandler::OnIndexSelected(int x, int y) const
{
	return std::make_unique<ReturnToMainGame>();
}

std::unique_ptr<EventHandler> LookHandler::Clone() const {
	return std::make_unique<LookHandler>(*this);
}