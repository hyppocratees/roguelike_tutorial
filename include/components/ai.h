#pragma once

#include "base_component.h"
#include <vector>
#include <utility>

class Engine;
class Entity;

class BaseAI : public BaseComponent {
public:
	BaseAI(Entity* entity): BaseComponent(entity) {}
	virtual void Perform(Engine& engine) {};
	virtual std::vector<std::pair<int, int>> GetPathTo(int dest_x, int dest_y);
	virtual int GetAiType() const { return 0; };
};

class HostileAI : public BaseAI {
public:
	HostileAI(Entity* entity) : BaseAI(entity) {};
	virtual void Perform(Engine& engine); 
	virtual int GetAiType() const { return 1; };
};