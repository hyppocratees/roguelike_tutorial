#pragma once

#include "input_handler.h"

#include <memory>

class Engine;


//enum for simple input handler
//handler more complex need direct creation
//TODO improve for more complex handler
enum HANDLER{
	MAINGAME,
	GAMEOVER,
	HISTORYVIEWER,
	ASKUSER,
	INVENTORY,
	INVENTORYACTIVATE,
	INVENTORYDROP,
	LOOK,
	POPUP,
	MAINMENU,
	SINGLERANGE,
	AREARANGE,
	LEVELUP
};

class HandlerFactory {
public:
	static std::unique_ptr<EventHandler> GetHandler(Engine& engine, HANDLER handler_enum);
};