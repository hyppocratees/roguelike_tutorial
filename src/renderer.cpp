#include <libtcod/libtcod.hpp>
#include <libtcod/console_drawing.h>
#include <format>
#include <string>
#include <utility>

#include "color.h"
#include "actor.h"
#include "renderer.h"
#include "gamemap.h"
#include "engine.h"
#include "entity_manager.h"
#include "item_manager.h"


void Renderer::RenderBar(tcod::Console& console, int c_value, int max_value, int total_width,const Actor* const player) {
	float percent_bar = ((float)c_value / (float)max_value);
	int bar_width = percent_bar * total_width;
	tcod::draw_rect(console, {0, 45, total_width, 1}, 0, std::nullopt, bar_empty);
	if (bar_width > 0) {
		tcod::draw_rect(console, { 0, 45, bar_width, 1 }, 0, std::nullopt, bar_filled);
	}
	tcod::print(console, { 1, 45 }, std::format("HP : {}/{}", player->GetHp(), player->GetMaxHp()), { bar_text }, { { 0, 0, 0 } }, TCOD_LEFT, TCOD_BKGND_NONE);
}

std::string Renderer::GetNamesAtLocation(int x, int y, const Engine& engine)
{
	if (!engine.GetMap()->Inbound(x, y) || !engine.GetMap()->IsInFov(x, y)) {
		return "";
	}
	const EntityManager& manager = engine.GetEntities();

	const ItemManager& item_manager = engine.GetItem();
	std::string names = "";
	for (const auto& entity : manager) {
		if (entity.GetX() == x && entity.GetY() == y){
			if (names == "") {
				names = entity.GetName();
				continue;
			}
			names = std::format("{}, {}", names, entity.GetName());
		}
	}
	for (const auto& entity : item_manager) {
		if (entity.GetX() == x && entity.GetY() == y) {
			if (names == "") {
				names = entity.GetName();
				continue;
			}
			names = std::format("{}, {}", names, entity.GetName());
		}
	}

	return names;
}

void Renderer::RenderNamesAtMouseLocation(const Engine& engine, int x, int y)
{
	std::pair<int, int> mouse_loc;
	mouse_loc = engine.GetMouseLocation();
	std::string names = GetNamesAtLocation(mouse_loc.first,mouse_loc.second, engine);
	tcod::print(engine.GetConsole(), { x, y }, names, white, black);
}

void Renderer::RenderDungeonLevel(const Engine& engine, int dungeon_level, int x, int y)
{
	std::string text = std::format("dungeon level : {}", dungeon_level);
	tcod::print(engine.GetConsole(), { x, y }, text, white, black);
}

