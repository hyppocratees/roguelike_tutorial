#include <libtcod.hpp>
#include <libtcod/color.hpp>
#include <SDL3/SDL.h>

#include <memory>
#include <vector>

#include "setup_game.h"
#include "glob_var.h"
#include "entity.h"
#include "engine.h"
#include "gamemap.h"
#include "procgen.h"
#include "color.h"

int main(int argc, char* argv[]) {

    SDL_SetEventEnabled(SDL_EVENT_TEXT_INPUT, true);

    tcod::Console console = tcod::Console{SCREEN_WIDTH, SCREEN_HEIGTH};  // Main console.

    // Configure the context.
    TCOD_ContextParams params = TCOD_ContextParams{};
    params.console = console.get();  // Derive the window size from the console size.
    params.window_title = "roguelike tutorial";
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.vsync = true;
    params.argc = argc;  // This allows some user-control of the context.
    params.argv = argv;

    // Tileset example using a Code Page 437 font.
    // "terminal8x8_gs_ro.png" must be in the working directory.
    auto tileset = tcod::load_tilesheet("../dejavu10x10_gs_tc.png", {32, 8}, tcod::CHARMAP_TCOD);
    params.tileset = tileset.get();

    Engine engine = NewGame(console, params);

    while (engine.IsRunning()) {  // Game loop.
        engine.HandleEvent();
        engine.Render();

    }
}