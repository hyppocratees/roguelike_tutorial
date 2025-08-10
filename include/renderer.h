#pragma once
#include <libtcod/libtcod.hpp>

class Actor;

class Renderer {
public:
	static void RenderBar(tcod::Console& console, int c_value, int max_value, int total_width,const Actor* const actor);
};