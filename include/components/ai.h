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
	virtual void Perform(Engine&) {};
	virtual std::vector<std::pair<int, int>> GetPathTo(int dest_x, int dest_y);
	virtual int GetAiType() const { return 0; };
	virtual std::unique_ptr<BaseAI> Clone() const;

	friend std::ostream& operator<<(std::ostream& os, const BaseAI& ai);

	virtual void ToText(std::ostream& os) const;

protected:
	Actor* entity_;
};

class HostileAI : public BaseAI {
public:
	HostileAI(Actor* entity) : BaseAI(entity) {};
	virtual void Perform(Engine& engine); 
	virtual int GetAiType() const { return 1; };
	virtual std::unique_ptr<BaseAI> Clone() const;
	virtual void ToText(std::ostream& os) const;
};

class DeadAI : public BaseAI {
public:
	DeadAI(Actor* entity) : BaseAI(entity) {};
	virtual void Perform(Engine&) {};
	virtual std::vector<std::pair<int, int>> GetPathTo(int, int) { return {}; };
	virtual int GetAiType() const { return -1; };
	virtual std::unique_ptr<BaseAI> Clone() const;
	virtual void ToText(std::ostream& os) const;
};

class ConfusedAI : public BaseAI {
public:
	ConfusedAI(Actor* entity, std::unique_ptr<BaseAI>& prevAI, int turn_remaining) : BaseAI(entity), turn_remaining_(turn_remaining) {
		prevAI_ = prevAI->Clone();
	}
	ConfusedAI(Actor* entity, BaseAI* prevAI, int turn_remaining) : BaseAI(entity), turn_remaining_(turn_remaining) {
		prevAI_ = prevAI->Clone();
	}
	virtual int GetAiType() const { return 2; };
	virtual std::unique_ptr<BaseAI> Clone() const;
	virtual void Perform(Engine& engine);
	virtual void ToText(std::ostream& os) const;
private:
	int turn_remaining_;
	std::unique_ptr<BaseAI> prevAI_;
};