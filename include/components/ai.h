#pragma once

#include "base_component.h"
#include <vector>
#include <utility>

class Engine;
class Actor;

class BaseAI {
public:
	BaseAI(Actor* entity): entity_(entity) {}
	virtual void Perform(Engine& engine) {};
	virtual std::vector<std::pair<int, int>> GetPathTo(int dest_x, int dest_y);
	virtual int GetAiType() const { return 0; };
protected:
	Actor* entity_;
};

class HostileAI : public BaseAI {
public:
	HostileAI(Actor* entity) : BaseAI(entity) {};
	virtual void Perform(Engine& engine); 
	virtual int GetAiType() const { return 1; };
};

class DeadAI : public BaseAI {
public:
	DeadAI(Actor* entity) : BaseAI(entity) {};
	virtual void Perform(Engine& engine) {};
	virtual std::vector<std::pair<int, int>> GetPathTo(int dest_x, int dest_y) { return {}; };
	virtual int GetAiType() const { return -1; };
};