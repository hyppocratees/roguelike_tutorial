#pragma once

#include "entity.h"

class BaseComponent {
public:
	BaseComponent(Entity& entity) : entity_(entity) {};
protected:
	Entity& entity_;
};