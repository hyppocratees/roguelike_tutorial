#pragma once
#include <libtcod/libtcod.hpp>
#include <string>

class Actor;
class Engine;

class Renderer {
public:
	static void RenderBar(tcod::Console& console, int c_value, int max_value, int total_width,const Actor* const actor);
	static std::string GetNamesAtLocation(int x, int y, const Engine& engine);
	static void RenderNamesAtMouseLocation(const Engine& engine, int x, int y);
	static void RenderDungeonLevel(const Engine& engine, int dungeon_level, int x, int y);
};