#pragma once
#include <memory>
#include <libtcod/libtcod.hpp>
#include <SDL3/SDL.h>

#include "action.h"

class Engine;

class EventHandler {
public:
	EventHandler(Engine& engine) : engine_(engine) {}
	virtual std::unique_ptr<Action> Dispatch() const;
	virtual void OnRender(tcod::Console& console) {};
	virtual std::unique_ptr<EventHandler> clone() const = 0;
protected:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const = 0;
	virtual void EvMouseMotion(const SDL_Event& event) const;
	Engine& engine_;
};

class MainGameEventHandler : public EventHandler {
public:
	MainGameEventHandler(Engine& engine) : EventHandler(engine) {}
	virtual std::unique_ptr<EventHandler> clone() const;
private:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
	
};

class GameOverEventHandler : public EventHandler {
public:
	GameOverEventHandler(Engine& engine) : EventHandler(engine) {}
	virtual std::unique_ptr<EventHandler> clone() const;
private:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
};

class HistoryViewerHandler : public EventHandler {
public:
	HistoryViewerHandler(Engine& engine);
	virtual void OnRender(tcod::Console& console);
	virtual std::unique_ptr<EventHandler> clone() const;
private:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
private:
	int log_length_;
	mutable int cursor_;
};