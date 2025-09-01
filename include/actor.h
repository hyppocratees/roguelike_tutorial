#pragma once
#include "entity.h"
#include "components/ai.h"
#include "components/fighter.h"
#include <memory>

#include "inventory.h"

class Actor : public Entity {
public:
	Actor() : Entity(), ai_(nullptr), fighter_(Fighter()), inv_(Inventory(0, this)) {
		inv_.SetOwner(this);
		fighter_.SetEntity(this);
	};
	Actor(int x, int y, char c, tcod::ColorRGB color, std::string name, bool block_mov, int ai_type, Fighter fighter, int rend_ord,const Inventory& inv) : Entity(x, y, c, color, name, block_mov, rend_ord), ai_(nullptr), fighter_(fighter), inv_(inv) {
		fighter_.SetEntity(this);
		inv_.SetOwner(this);
		switch (ai_type) {
		case 1:
			ai_ = std::make_unique<HostileAI>(this);
			break;
		case -1:
			ai_ = std::make_unique<DeadAI>(this);
			break;
		case 0:
		default:
			ai_ = std::make_unique<BaseAI>(this);
			break;
		}
	};
	Actor(const Actor& actor) : Entity(actor.x_, actor.y_, actor.char_, actor.color_, actor.name_, actor.block_mov_, actor.rend_ord_), ai_(nullptr), fighter_(actor.fighter_), inv_(actor.inv_){
		int ai_type = actor.ai_->GetAiType();
		switch (ai_type) {
		case 1:
			ai_ = std::make_unique<HostileAI>(this);
			break;
		case -1:
			ai_ = std::make_unique<DeadAI>(this);
			break;
		case 0:
		default:
			ai_ = std::make_unique<BaseAI>(this);
			break;
		}
		fighter_.SetEntity(this);
	};

	Actor& operator=(const Actor& other) { 
		if (this == &other) return *this;
		x_ = other.x_;
		y_ = other.y_;
		char_ = other.char_;
		color_ = other.color_;
		name_ = other.name_;
		block_mov_ = other.block_mov_;
		map_ = other.map_; //Actor does not own map
		rend_ord_ = other.rend_ord_;
		fighter_ = other.fighter_;
		inv_ = other.inv_;
		inv_.SetOwner(this);
		int ai_type = other.ai_->GetAiType();
		switch (ai_type) {
		case 1:
			ai_ = std::make_unique<HostileAI>(this);
			break;
		case -1:
			ai_ = std::make_unique<DeadAI>(this);
			break;
		case 0:
		default:
			ai_ = std::make_unique<BaseAI>(this);
			break;
		}
		return *this;
	};

	bool IsAlive() const { return fighter_.GetHp() != 0; };
	BaseAI* GetAI() { return ai_.get(); };
	int GetPower() const { return fighter_.GetPower(); }
	int GetDefense() const { return fighter_.GetDefense(); };
	int GetHp() const { return fighter_.GetHp(); };
	int GetMaxHp() const { return fighter_.GetMaxHp(); };
	void SetAI(const std::unique_ptr<BaseAI>& ai) { if (ai == nullptr) { ai_ = nullptr; } else { ai_ = std::make_unique<BaseAI>(*ai); } };
	void TakeDamage(int damage) { fighter_.Hp(fighter_.GetHp() - damage); };
	void Die() { fighter_.Die(*this); };
	Fighter& GetFighter() { return fighter_; };
	[[nodiscard]] Inventory& GetInventory() { return inv_; };
protected:
	std::unique_ptr<BaseAI> ai_;
	Fighter fighter_;
	Inventory inv_;
};
