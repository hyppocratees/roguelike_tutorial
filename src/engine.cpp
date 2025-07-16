#include "engine.h"
#include "input_handler.h"


Engine::Engine(std::vector<Entity>& entities, EventHandler handler, Entity& player, tcod::Context& context, tcod::Console& console) : entities_(entities), handler_(handler), player_(player), context_(context), console_(console)
{
}

void Engine::HandleEvent()
{
	std::unique_ptr<Action> action = handler_.Dispatch();

	if (action) {
		action->Execute(player_);
	}

}

void Engine::Render()
{
	for (Entity& entity : entities_) {
		console_.at(entity.GetX(), entity.GetY()).ch = entity.GetChar();
		console_.at(entity.GetX(), entity.GetY()).fg = entity.GetColor();
	}

	context_.present(console_);

	console_.clear();

}
