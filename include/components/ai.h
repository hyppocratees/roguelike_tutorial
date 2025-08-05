#pragma once

#include "base_component.h"
#include <vector>
#include <utility>

class Entity;

class BaseAI : public BaseComponent {
	BaseAI(Entity& entity): BaseComponent(entity) {}
	virtual void Perform() = 0;
	virtual std::vector<std::pair<int, int>> GetPathTo(int dest_x, int dest_y);
};