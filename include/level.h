#pragma once

#include <iostream>

class Engine;
class Actor;

class Level {
public:
	Level() {};
	Level(Actor* actor, int c_level = 1, int c_xp = 0, int level_up_base = 0, int level_up_factor = 150, int xp_given = 0) : actor_(actor), c_level_(c_level), c_xp_(c_xp), level_up_base_(level_up_base), level_up_factor_(level_up_factor), xp_given_(xp_given) {};
	Level(Actor* actor, const Level& lvl) : actor_(actor), c_level_(lvl.c_level_), c_xp_(lvl.c_xp_), level_up_base_(lvl.level_up_base_), level_up_factor_(lvl.level_up_factor_), xp_given_(lvl.xp_given_) {};

	inline int XpToNextLevel() const { return c_level_ * level_up_factor_ + level_up_base_; };
	inline bool CanLevelUp() const { return c_xp_ > XpToNextLevel(); };
	void GainXp(Engine& engine, int xp);
	void IncreaseLevel(Engine& engine);
	void IncreaseMaxHp(Engine& engine, int amount);
	void IncreasePower(Engine& engine, int amount);
	void IncreaseDefense(Engine& engine, int amount);

	int GetXp() const { return xp_given_; };
	int GetLevel() const { return c_level_; };
	int GetCXp() const { return c_xp_; };

	void SetActor(Actor* actor) { actor_ = actor; };

	friend std::ostream& operator<<(std::ostream& os, const Level& lvl);
	friend std::istream& operator>>(std::istream& is, Level& lvl);


private:
	Actor* actor_ = nullptr;
	int c_level_ = 1;
	int c_xp_ = 0;
	int level_up_base_ = 0;
	int level_up_factor_ = 150;
	int xp_given_ = 0;

};