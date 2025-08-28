#pragma once

class Entity;

class BaseComponent {
public:
	BaseComponent() : entity_(nullptr) {};
	BaseComponent(Entity* entity) : entity_(entity) {};
	virtual void SetEntity(Entity* entity) { entity_ = entity; };
protected:
	Entity* entity_;
};