#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <libtcod.hpp>

int main(int argc, char* argv[]) {
  tcod::Console console = tcod::Console{80, 25};  // Main console.

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
  // auto tileset = tcod::load_tilesheet("terminal8x8_gs_ro.png", {16, 16}, tcod::CHARMAP_CP437);
  // params.tileset = tileset.get();

  tcod::Context context = tcod::Context(params);

  while (true) {  // Game loop.
    console.clear();
    tcod::print(console, {0, 0}, "Hello World", std::nullopt, std::nullopt);
    context.present(console);  // Updates the visible display.

    SDL_Event event;
    SDL_WaitEvent(nullptr);  // Optional, sleep until events are available.
    while (SDL_PollEvent(&event)) {
      context.convert_event_coordinates(event);  // Optional, converts pixel coordinates into tile coordinates.
      switch (event.type) {
        case SDL_EVENT_QUIT:
          return 0;  // Exit.
      }
    }
  }
}