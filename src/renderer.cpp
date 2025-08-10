#include <libtcod/libtcod.hpp>
#include <libtcod/console_drawing.h>
#include <format>

#include "color.h"
#include "actor.h"
#include "renderer.h"


void Renderer::RenderBar(tcod::Console& console, int c_value, int max_value, int total_width,const Actor* const player) {
	float percent_bar = ((float)c_value / (float)max_value);
	int bar_width = percent_bar * total_width;
	tcod::draw_rect(console, {0, 45, total_width, 1}, 0, std::nullopt, bar_empty);
	if (bar_width > 0) {
		tcod::draw_rect(console, { 0, 45, bar_width, 1 }, 0, std::nullopt, bar_filled);
	}
	tcod::print(console, { 1, 45 }, std::format("HP : {}/{}", player->GetHp(), player->GetMaxHp()), { bar_text }, { { 0, 0, 0 } }, TCOD_LEFT, TCOD_BKGND_NONE);
}
