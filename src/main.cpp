#include <libtcod.hpp>

#include <memory>

#include "glob_var.h"
#include "action.h"
#include "input_handler.h"
#include "player.h"

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

    Player player(int(SCREEN_WIDTH / 2), int(SCREEN_HEIGTH / 2));

    EventHandler handler;
    std::unique_ptr<Action> action{ nullptr };

    while (true) {  // Game loop.
        console.clear();
        tcod::print(console, {player.Getx(), player.Gety()}, "@", std::nullopt, std::nullopt);

        action = handler.Dispatch();
        if (action) {
            action->Execute(player);
        }

        context.present(console);  // Updates the visible display.

    }
}