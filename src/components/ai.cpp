#include "components/ai.h"
#include "entity.h"
#include "gamemap.h"
#include "actor.h"
#include "engine.h"
#include "action.h"

#include <vector>
#include <utility>
#include <memory>
#include <string>
#include <libtcod/path.hpp>

std::vector<std::pair<int, int>> BaseAI::GetPathTo(int dest_x, int dest_y)
{
	std::vector<std::pair<int, int>> result_path;
	TCODPath path(entity_->GetMap()->GetMap());
	path.compute(entity_->GetX(), entity_->GetY(), dest_x, dest_y);

	for (int i = 0; i < path.size(); ++i) {
		int x = 0;
		int y = 0;
		path.get(i, &x, &y);
		result_path.emplace_back(x, y);
	}
	return result_path;
}

void HostileAI::Perform(Engine& engine) {
	Actor* target = engine.GetPlayer();
	int dx = target->GetX() - entity_->GetX();
	int dy = target->GetY() - entity_->GetY();
	int distance = std::max(std::abs(dx), std::abs(dy));
	std::vector<std::pair<int, int>> path{};
	if (engine.GetMap()->IsInFov(entity_->GetX(), entity_->GetY())) {
		if (distance <= 1) return MeleeAction(*entity_, dx, dy).Perform(engine);
		path = GetPathTo(target->GetX(), target->GetY());
	}
	if (!path.empty()) {
		int dest_x = path.at(0).first; 
		int dest_y = path.at(0).second;
		return MovementAction(*entity_, dest_x - entity_->GetX(), dest_y - entity_->GetY()).Perform(engine);
	}
	return WaitAction(*entity_).Perform(engine);
}

std::unique_ptr<BaseAI> BaseAI::Clone() const {
	return std::make_unique<BaseAI>(entity_);
}

std::unique_ptr<BaseAI> HostileAI::Clone() const {
	return std::make_unique<HostileAI>(entity_);
}

std::unique_ptr<BaseAI> DeadAI::Clone() const {
	return std::make_unique<DeadAI>(entity_);
}

std::unique_ptr<BaseAI> ConfusedAI::Clone() const {
	return std::make_unique<ConfusedAI>(entity_, prevAI_.get(), turn_remaining_);
}

void ConfusedAI::Perform(Engine& engine) {
	if (turn_remaining_ <= 0) {
		std::string name = entity_->GetName();
		engine.AddMessage(std::format("{} is no longer confused", name), white, false);
		entity_->SetAI(prevAI_);
		return;
	}
	auto* rand = TCODRandom::getInstance();
	int rx = rand->getInt(-1, 1);
	int ry = rand->getInt(-1, 1);
	turn_remaining_ -= 1;
	auto action = BumpAction(*entity_, rx, ry);
	action.Perform(engine);
}

void BaseAI::ToText(std::ostream& os) const {
	os << 0;
}

void HostileAI::ToText(std::ostream& os) const {
	os << 1;
}

void ConfusedAI::ToText(std::ostream& os) const {
	os << 2 << "\n";
	os << turn_remaining_ << " ";
	prevAI_->ToText(os);
}

void DeadAI::ToText(std::ostream& os) const {
	os << -1 << "\n";
}


std::ostream& operator<<(std::ostream& os, const BaseAI& ai)
{
	ai.ToText(os);
	return os;
}
