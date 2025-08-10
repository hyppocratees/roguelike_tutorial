#pragma once
#include <memory>
#include <SDL3/SDL.h>

#include "action.h"

class Engine;

class EventHandler {
public:
	EventHandler(Engine& engine) : engine_(engine) {}
	virtual std::unique_ptr<Action> Dispatch() const;
protected:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const = 0;
	Engine& engine_;
};

class MainGameEventHandler : public EventHandler {
public:
	MainGameEventHandler(Engine& engine) : EventHandler(engine) {}
private:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
};

class GameOverEventHandler : public EventHandler {
public:
	GameOverEventHandler(Engine& engine) : EventHandler(engine) {}
private:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
};