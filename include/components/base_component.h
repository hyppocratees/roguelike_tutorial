#pragma once

class Actor;

class BaseComponent {
public:
	BaseComponent() : entity_(nullptr) {};
	BaseComponent(Actor* entity) : entity_(entity) {};
protected:
	Actor* entity_;
};