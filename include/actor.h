#pragma once
#include "entity.h"
#include "components/ai.h"
#include "components/fighter.h"
#include <memory>

class Actor : public Entity {
public:
	Actor() : Entity(), ai_(nullptr), fighter_(Fighter()) {};
	Actor(int x, int y, char c, tcod::ColorRGB color, std::string name, bool block_mov, int ai_type, Fighter fighter) : Entity(x, y, c, color, name, block_mov), ai_(nullptr), fighter_(fighter) {
		fighter_.SetEntity(this);
		switch (ai_type) {
		case 1:
			ai_ = std::make_unique<HostileAI>(this);
			break;
		
		case 0:
		default:
			ai_ = std::make_unique<BaseAI>(this);
			break;
		}
	};
	Actor(const Actor& actor) : Entity(actor.x_, actor.y_, actor.char_, actor.color_, actor.name_, actor.block_mov_), ai_(nullptr), fighter_(actor.fighter_) {
		int ai_type = actor.ai_->GetAiType();
		switch (ai_type) {
		case 1:
			ai_ = std::make_unique<HostileAI>(this);
			break;

		case 0:
		default:
			ai_ = std::make_unique<BaseAI>(this);
			break;
		}
		fighter_.SetEntity(this);
	};
	bool IsAlive() const { return fighter_.GetHp() != 0; };
	BaseAI* GetAI() { return ai_.get(); };
	int GetPower() const { return fighter_.GetPower(); }
	int GetDefense() const { return fighter_.GetDefense(); };
	int GetHp() const { return fighter_.GetHp(); };
	int GetMaxHp() const { return fighter_.GetMaxHp(); };
	void SetAI(const std::unique_ptr<BaseAI>& ai) { if (ai == nullptr) { ai_ = nullptr; } else { ai_ = std::make_unique<BaseAI>(*ai); } };
	void TakeDamage(int damage) { fighter_.Hp(fighter_.GetHp() - damage); };
protected:
	std::unique_ptr<BaseAI> ai_;
	Fighter fighter_;
};
