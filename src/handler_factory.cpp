#include "handler_factory.h"
#include "setup_game.h"
#include "engine.h"

std::unique_ptr<EventHandler> HandlerFactory::GetHandler(Engine& engine, HANDLER handler_enum)
{
	switch (handler_enum) {
	case MAINGAME:
		return std::make_unique<MainGameEventHandler>(engine);
	case GAMEOVER:
		return std::make_unique<GameOverEventHandler>(engine);
	case HISTORYVIEWER:
		return std::make_unique<HistoryViewerHandler>(engine);
	case ASKUSER:
		return std::make_unique<AskUserEventHandler>(engine);
	case INVENTORY:
		return std::make_unique<InventoryEventHandler>(engine);
	case INVENTORYACTIVATE:
		return std::make_unique<InventoryActivateHandler>(engine);
	case INVENTORYDROP:
		return std::make_unique<InventoryDropHandler>(engine);
	case LOOK:
		return std::make_unique<LookHandler>(engine);
	case MAINMENU:
		return std::make_unique<MainMenu>(engine);
	default:
		return std::make_unique<MainGameEventHandler>(engine);
	}
}
