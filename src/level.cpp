#include "level.h"
#include "engine.h"
#include "color.h"


void Level::GainXp(Engine& engine, int xp) {
	if (xp == 0 || level_up_base_ == 0) return;

	c_xp_ += xp;

	engine.AddMessage(std::format("you earned {} experience points", xp), yellow, false);

	if (CanLevelUp()) engine.AddMessage(std::format("you can advance to level {}", c_level_ + 1), yellow, false);

}

void Level::IncreaseLevel(Engine& engine) {
	c_xp_ -= XpToNextLevel();
	c_level_++;
	engine.AddMessage(std::format("you successfully reached level {}", c_level_), yellow, false);
}

void Level::IncreaseMaxHp(Engine& engine, int amount) {
	actor_->GetFighter().GainMaxHp(amount);
	actor_->GetFighter().Heal(amount);
	engine.AddMessage(std::format("you gained {} Hp", amount), yellow, false);
	IncreaseLevel(engine);
}

void Level::IncreasePower(Engine& engine, int amount) {
	actor_->GetFighter().GainPower(amount);
	engine.AddMessage(std::format("you gained {} attack strength", amount), yellow, false);
	IncreaseLevel(engine);
}

void Level::IncreaseDefense(Engine& engine, int amount) {
	actor_->GetFighter().GainDefense(amount);
	engine.AddMessage(std::format("you gained {} defense", amount), yellow, false);
	IncreaseLevel(engine);
}

std::ostream& operator<<(std::ostream& os, const Level& lvl) {
	os << lvl.c_level_ << " " << lvl.c_xp_ << " " << lvl.level_up_base_ << " " << lvl.level_up_factor_ << " " << lvl.xp_given_;
	return os;
}

std::istream& operator>>(std::istream& is, Level& lvl) {
	is >> lvl.c_level_ >> lvl.c_xp_ >> lvl.level_up_base_ >> lvl.level_up_factor_ >> lvl.xp_given_;
	return is;
}