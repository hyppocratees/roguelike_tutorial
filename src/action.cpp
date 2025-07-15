#include "action.h"
#include "player.h"

MovementAction::MovementAction(int dx, int dy) : Action(), dx_(dx), dy_(dy) {
}

void MovementAction::Execute(Player& player) const{ 
	player.move(dx_, dy_);
}