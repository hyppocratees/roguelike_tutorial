#include "components/ai.h"
#include "entity.h"
#include "gamemap.h"
#include "actor.h"
#include "engine.h"
#include "action.h"

#include <vector>
#include <utility>
#include <libtcod/path.hpp>

std::vector<std::pair<int, int>> BaseAI::GetPathTo(int dest_x, int dest_y)
{
	std::vector<std::pair<int, int>> result_path;
	TCODPath path(entity_->GetMap()->GetMap());
	path.compute(entity_->GetX(), entity_->GetY(), dest_x, dest_y);

	for (int i = 0; i < path.size(); ++i) {
		int* x = 0;
		int* y = 0;
		path.get(i, x, y);
		result_path.emplace_back(*x, *y);
	}
	return result_path;
}

void HostileAI::Perform(Engine& engine) {
	Actor* target = engine.GetPlayer();
	int dx = target->GetX() - entity_->GetX();
	int dy = target->GetY() - entity_->GetY();
	int distance = std::max(std::abs(dx), std::abs(dy));
	std::vector<std::pair<int, int>> path{};
	if (engine.GetMap().IsInFov(entity_->GetX(), entity_->GetY())) {
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