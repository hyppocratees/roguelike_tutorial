#include <libtcod.hpp>
#include <libtcod/color.hpp>

#include <memory>
#include <vector>

#include "glob_var.h"
#include "entity.h"
#include "engine.h"
#include "gamemap.h"
#include "procgen.h"
#include "color.h"

int main(int argc, char* argv[]) {
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

    tcod::Context context = tcod::Context(params);

    GameMap gamemap(MAP_WIDTH, MAP_HEIGTH);

    MapGenerator mapgen(MAP_WIDTH, MAP_HEIGTH, ROOM_MIN_SIZE, ROOM_MAX_SIZE, MAX_ROOMS);

    Engine engine(context, console, gamemap, mapgen);

    engine.AddMessage("Hello and welcome, adventurer, to yet another dungeon", welcome_text);

    while (engine.IsRunning()) {  // Game loop.
        engine.HandleEvent();
        engine.Render();

    }
}