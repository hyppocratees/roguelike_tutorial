#pragma once
#include <memory>
#include <utility>
#include <functional>
#include <string>
#include <libtcod/libtcod.hpp>

#include "action.h"

class Engine;
enum HANDLER;

class EventHandler {
public:
	EventHandler(Engine& engine) : engine_(engine) {}
	virtual std::unique_ptr<Action> Dispatch() const;
	virtual void OnRender(tcod::Console&) const {};
	virtual std::unique_ptr<EventHandler> Clone() const = 0;
	virtual HANDLER Type() const = 0;
protected:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const = 0;
	virtual std::unique_ptr<Action> EvMouseMotion(const SDL_Event& event) const;
	Engine& engine_;
};

class MainGameEventHandler : public EventHandler {
public:
	MainGameEventHandler(Engine& engine) : EventHandler(engine) {}
	virtual std::unique_ptr<EventHandler> Clone() const;
	virtual HANDLER Type() const;
protected:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
	
};

class GameOverEventHandler : public EventHandler {
public:
	GameOverEventHandler(Engine& engine) : EventHandler(engine) {}
	virtual std::unique_ptr<EventHandler> Clone() const;
	virtual HANDLER Type() const;
protected:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
};

class HistoryViewerHandler : public EventHandler {
public:
	HistoryViewerHandler(Engine& engine);
	virtual void OnRender(tcod::Console& console) const;
	virtual std::unique_ptr<EventHandler> Clone() const;
	virtual HANDLER Type() const;
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
	virtual void OnRender(tcod::Console&) const {};
	virtual HANDLER Type() const;
protected:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
};

class InventoryEventHandler : public AskUserEventHandler {
public:
	InventoryEventHandler(Engine& engine);
	InventoryEventHandler(Engine& engine, std::string title);
	virtual void OnRender(tcod::Console& console) const;
	virtual std::unique_ptr<EventHandler> Clone() const;
	virtual HANDLER Type() const;
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
	virtual HANDLER Type() const;
protected:
	virtual std::unique_ptr<Action> OnItemSelected(Item* item) const;
};

class InventoryDropHandler : public InventoryEventHandler {
public:
	InventoryDropHandler(Engine& engine);
	virtual std::unique_ptr<EventHandler> Clone() const;
	virtual HANDLER Type() const;
protected:
	virtual std::unique_ptr<Action> OnItemSelected(Item* item) const;

};

class SelectIndexHandler : public AskUserEventHandler {
public: 
	SelectIndexHandler(Engine& engine);
	virtual std::unique_ptr<EventHandler> Clone() const = 0;
	virtual void OnRender(tcod::Console& console) const;
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
	virtual HANDLER Type() const;
protected:
	virtual std::unique_ptr<Action> OnIndexSelected(int x, int y) const;
};

class SingleRangedAttackHandler : public SelectIndexHandler {
public:
	SingleRangedAttackHandler(Engine& engine, std::function<void(int, int)> callback) : SelectIndexHandler(engine), callback_(callback) {};
	virtual std::unique_ptr<EventHandler> Clone() const;
	virtual HANDLER Type() const;
protected:
	virtual std::unique_ptr<Action> OnIndexSelected(int x, int y) const;
private:
	std::function<void(int, int)> callback_;
};

class AreaRangedAttackHandler : public SelectIndexHandler {
public:
	AreaRangedAttackHandler(Engine& engine, int radius, std::function<void(int, int)> callback) : SelectIndexHandler(engine), radius_(radius), callback_(callback) {};
	virtual std::unique_ptr<EventHandler> Clone() const;
	virtual void OnRender(tcod::Console& console) const; 
	virtual HANDLER Type() const;
protected:
		virtual std::unique_ptr<Action> OnIndexSelected(int x, int y) const;
private:
	std::function<void(int, int)> callback_;
	int radius_;
};

class PopupMessage : public EventHandler {
public:
	PopupMessage(Engine& engine, std::string text, std::unique_ptr<EventHandler>& parent_handler) : EventHandler(engine), text_(text), parent_handler_(std::move(parent_handler)) {};
	PopupMessage(const PopupMessage& popupmessage) : EventHandler(popupmessage.engine_), text_(popupmessage.text_), parent_handler_(popupmessage.parent_handler_->Clone()) {}
	
	virtual void OnRender(tcod::Console& console) const; 
	virtual std::unique_ptr<EventHandler> Clone() const;
	virtual HANDLER Type() const;
protected:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
private:
	std::string text_; 
	std::unique_ptr<EventHandler> parent_handler_;
};

class LevelUpEventHandler : public AskUserEventHandler {
public:
	LevelUpEventHandler(Engine& engine) : AskUserEventHandler(engine) {};
	virtual void OnRender(tcod::Console& cons) const;
	virtual HANDLER Type() const;
	virtual std::unique_ptr<EventHandler> Clone() const;
protected:
	virtual std::unique_ptr<Action> EvKeydown(const SDL_Event& event) const;
};

class CharacterScreenHandler : public AskUserEventHandler {
public:
	CharacterScreenHandler(Engine& engine) : AskUserEventHandler(engine) {}
	virtual std::unique_ptr<EventHandler> Clone() const;
	virtual HANDLER Type() const;
	virtual void OnRender(tcod::Console& cons) const;
};
