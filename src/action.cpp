#include "action.h"
#include "entity.h"

MovementAction::MovementAction(int dx, int dy) : Action(), dx_(dx), dy_(dy) {
}

void MovementAction::Execute(Entity& player) const{ 
	player.Move(dx_, dy_);
}