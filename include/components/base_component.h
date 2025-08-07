#pragma once

#include "entity.h"

class BaseComponent {
public:
	BaseComponent() : entity_(nullptr) {};
	BaseComponent(Entity* entity) : entity_(entity) {};
protected:
	Entity* entity_;
};