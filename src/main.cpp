#include <libtcod.hpp>
#include <libtcod/color.hpp>

#include <memory>
#include <vector>

#include "glob_var.h"
#include "entity.h"
#include "engine.h"
#include "gamemap.h"
#include "procgen.h"

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

    MapGenerator mapgen(MAP_WIDTH, MAP_HEIGTH);

    mapgen.Generate(gamemap);

    Entity player(int(SCREEN_WIDTH / 2), int(SCREEN_HEIGTH / 2), '@', tcod::ColorRGB(255, 255, 255));
    Entity npc(int(SCREEN_WIDTH / 2 - 5), int(SCREEN_HEIGTH / 2), '@', tcod::ColorRGB(255, 255, 0));

    std::unique_ptr<Action> action{ nullptr };

    std::vector<Entity> entities{ player, npc };

    Entity& player_ref = entities[0];

    Engine engine(entities, player_ref, context, console, gamemap);

    while (engine.IsRunning()) {  // Game loop.
        engine.HandleEvent();
        engine.Render();

    }
}