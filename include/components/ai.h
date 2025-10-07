#pragma once

#include "base_component.h"
#include <vector>
#include <utility>
#include <memory>

class Engine;
class Actor;

class BaseAI {
public:
	BaseAI(Actor* entity) : entity_(entity) {};
	virtual void Perform(Engine& engine) {};
	virtual std::vector<std::pair<int, int>> GetPathTo(int dest_x, int dest_y);
	virtual int GetAiType() const { return 0; };
	virtual std::unique_ptr<BaseAI> Clone() const;
protected:
	Actor* entity_;
};

class HostileAI : public BaseAI {
public:
	HostileAI(Actor* entity) : BaseAI(entity) {};
	virtual void Perform(Engine& engine); 
	virtual int GetAiType() const { return 1; };
	virtual std::unique_ptr<BaseAI> Clone() const;
};

class DeadAI : public BaseAI {
public:
	DeadAI(Actor* entity) : BaseAI(entity) {};
	virtual void Perform(Engine& engine) {};
	virtual std::vector<std::pair<int, int>> GetPathTo(int dest_x, int dest_y) { return {}; };
	virtual int GetAiType() const { return -1; };
	virtual std::unique_ptr<BaseAI> Clone() const;
};

class ConfusedAI : public BaseAI {
public:
	ConfusedAI(Actor* entity, std::unique_ptr<BaseAI>& prevAI, int turn_remaining) : BaseAI(entity), turn_remaining_(turn_remaining) {
		prevAI_ = prevAI->Clone();
	}
	ConfusedAI(Actor* entity, BaseAI* prevAI, int turn_remaining) : BaseAI(entity), turn_remaining_(turn_remaining) {
		prevAI_ = prevAI->Clone();
	}

	virtual std::unique_ptr<BaseAI> Clone() const;
	virtual void Perform(Engine& engine);
private:
	int turn_remaining_;
	std::unique_ptr<BaseAI> prevAI_;
};