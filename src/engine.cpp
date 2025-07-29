#include "engine.h"
#include "input_handler.h"
#include "gamemap.h"
#include "glob_var.h"

Engine::Engine(std::vector<Entity>& entities, Entity& player, tcod::Context& context, tcod::Console& console, GameMap& map) : entities_(entities), handler_(EventHandler(*this)), player_(player), context_(context), console_(console), map_(map), isrunning_(true)
{
	UpdateFov();
}

void Engine::HandleEvent()
{
	std::unique_ptr<Action> action = handler_.Dispatch();

	if (action) {
		action->Perform(*this, player_);
		UpdateFov();
	}

}

void Engine::Render()
{
	map_.Render(console_);

	for (Entity& entity : entities_) {
		console_.at(entity.GetX(), entity.GetY()).ch = entity.GetChar();
		console_.at(entity.GetX(), entity.GetY()).fg = entity.GetColor();
	}

	context_.present(console_);

	console_.clear();

}

void Engine::UpdateFov() const
{
	map_.UpdateFov(player_.GetX(), player_.GetY(), FOV_RADIUS);
}
