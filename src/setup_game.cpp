#include "setup_game.h"
#include "engine.h"
#include "glob_var.h"
#include "entity_factory.h"
#include "color.h"
#include "action.h"
#include "handler_factory.h"

#include <memory>
#include <libtcod.hpp>
#include <SDL3/SDL_events.h>

Engine NewGame(tcod::Console& console, TCOD_ContextParams& params)
{
    tcod::Context context = tcod::Context(params);

    GameMap gamemap(MAP_WIDTH, MAP_HEIGTH);

    MapGenerator mapgen(MAP_WIDTH, MAP_HEIGTH, ROOM_MIN_SIZE, ROOM_MAX_SIZE, MAX_ROOMS);

    Engine engine(context, console, gamemap, mapgen);

    engine.AddMessage("Hello and welcome, adventurer, to yet another dungeon", welcome_text);

    return engine;

}

void MainMenu::OnRender(tcod::Console& console) {
    tcod::draw_quartergraphics(console, background_);
    //background_.blit2x(console, 0, 0);
    tcod::print(console, { console.get_width() / 2, console.get_height() / 2 - 4 }, "TOMB OF THE ANCIENT KINGS", menu_title, black, TCOD_CENTER, TCOD_BKGND_NONE);
    tcod::print(console, { console.get_width() / 2, console.get_height() - 2 }, "By hyppocratees", menu_title, black, TCOD_CENTER, TCOD_BKGND_NONE);
    tcod::print(console, { console.get_width() / 2, console.get_height() / 2 - 2 }, "[N] Play a new game", menu_text, black, TCOD_CENTER, TCOD_BKGND_NONE);
    tcod::print(console, { console.get_width() / 2, console.get_height() / 2 - 1 }, "[C] Continue last game", menu_text, black, TCOD_CENTER, TCOD_BKGND_NONE);
    tcod::print(console, { console.get_width() / 2, console.get_height() / 2 }, "[Q] Quit", menu_text, black, TCOD_CENTER, TCOD_BKGND_NONE);
}

std::unique_ptr<EventHandler> MainMenu::Clone() const {
    return std::make_unique<MainMenu>(*this);
}

std::unique_ptr<Action> MainMenu::EvKeydown(const SDL_Event& event) const {
    std::unique_ptr<Action> action = nullptr;

    switch (event.key.key) {
    case SDLK_Q:
        return std::make_unique<EscapeAction>();
    case SDLK_C:
        return std::make_unique<LoadAction>();
    case SDLK_N:
        return std::make_unique<ReturnToMainGame>();
    }
    return action;
}

HANDLER MainMenu::Type() const
{
    return MAINMENU;
}