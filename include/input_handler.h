#pragma once
#include <memory>
#include <utility>
#include <libtcod/libtcod.hpp>
#include <SDL3/SDL.h>

#include "action.h"

class Engine;

class EventHandler {
public:
	EventHandler(Engine& engine) : engine_(engine) {}
	virtual std::unique_ptr<Action> Dispatch() const;
	virtual void OnRender(tcod::Console& console) {};
	virtual std::unique_ptr<EventHandler> Clone() const = 0;
protected:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const = 0;
	virtual std::unique_ptr<Action> EvMouseMotion(const SDL_Event& event) const;
	Engine& engine_;
};

class MainGameEventHandler : public EventHandler {
public:
	MainGameEventHandler(Engine& engine) : EventHandler(engine) {}
	virtual std::unique_ptr<EventHandler> Clone() const;
protected:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
	
};

class GameOverEventHandler : public EventHandler {
public:
	GameOverEventHandler(Engine& engine) : EventHandler(engine) {}
	virtual std::unique_ptr<EventHandler> Clone() const;
protected:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
};

class HistoryViewerHandler : public EventHandler {
public:
	HistoryViewerHandler(Engine& engine);
	virtual void OnRender(tcod::Console& console);
	virtual std::unique_ptr<EventHandler> Clone() const;
protected:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
private:
	int log_length_;
	mutable int cursor_;
};

class AskUserEventHandler : public EventHandler {
public:
	AskUserEventHandler(Engine& engine);
	virtual std::unique_ptr<EventHandler> Clone() const;
protected:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
};

class InventoryEventHandler : public AskUserEventHandler {
public:
	InventoryEventHandler(Engine& engine);
	InventoryEventHandler(Engine& engine, std::string title);
	virtual void OnRender(tcod::Console& console);
	virtual std::unique_ptr<EventHandler> Clone() const;
protected:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
	virtual std::unique_ptr<Action> OnItemSelected(Item* item) const;
protected:
	std::string title_{ "<missing title>" };
};

class InventoryActivateHandler : public InventoryEventHandler {
public:
	InventoryActivateHandler(Engine& engine);
	virtual std::unique_ptr<EventHandler> Clone() const;
protected:
	virtual std::unique_ptr<Action> OnItemSelected(Item* item) const;
};

class InventoryDropHandler : public InventoryEventHandler {
public:
	InventoryDropHandler(Engine& engine);
	virtual std::unique_ptr<EventHandler> Clone() const;
protected:
	virtual std::unique_ptr<Action> OnItemSelected(Item* item) const;

};

class SelectIndexHandler : public AskUserEventHandler {
public: 
	SelectIndexHandler(Engine& engine);
	virtual std::unique_ptr<EventHandler> Clone() const = 0;
	virtual void OnRender(tcod::Console& console);
protected:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
	virtual std::unique_ptr<Action> EvMouseMotion(const SDL_Event& event) const;
	virtual std::unique_ptr<Action> OnIndexSelected(int x, int y) const = 0;
	virtual std::unique_ptr<Action> OnIndexSelected(std::pair<int, int> xy) const {
		return OnIndexSelected(xy.first, xy.second); };
protected:
	std::pair<int, int>& mouse_location_;
};

class LookHandler : public SelectIndexHandler {
public:
	LookHandler(Engine& engine) : SelectIndexHandler(engine) {};
	virtual std::unique_ptr<EventHandler> Clone() const;
protected:
	virtual std::unique_ptr<Action> OnIndexSelected(int x, int y) const;
};