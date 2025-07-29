#include "action.h"
#include "entity.h"
#include "gamemap.h"
#include "engine.h"

void EscapeAction::Perform(Engine& engine, Entity& player) const {
	engine.Quit();
}

MovementAction::MovementAction(int dx, int dy) : Action(), dx_(dx), dy_(dy) {
}

void MovementAction::Perform(Engine& engine, Entity& player) const{
	int dest_x = player.GetX() + dx_;
	int dest_y = player.GetY() + dy_;
	if (!engine.GetMap().Inbound(dest_x, dest_y)) {
		return;
	}
	if (!engine.GetMap().GetTile(dest_x, dest_y).walkable) {
		return;
	}
	player.Move(dx_, dy_);
}