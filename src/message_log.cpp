#include "message_log.h"

#include <string>
#include <format>
#include <vector>
#include <libtcod/console_printing.hpp>

std::string Message::FullText() const {
	if (count_ > 1) {
		return std::format("{} (x{})", text_, count_);
	}
	return text_;
}

void MessageLog::AddMessage(std::string text, tcod::ColorRGB color, bool stack)
{
	if (stack && !messages_.empty()) {
		if (messages_.back().GetText() == text) {
			messages_.back().IncreaseCount();
			return;
		}
	}
	messages_.emplace_back(text, color);
}

void MessageLog::Render(tcod::Console& console, int x, int y, int width, int height, std::vector<Message> message)
{
	int y_offset = height - 1;
	for (std::vector<Message>::reverse_iterator r_iter = message.rbegin(); r_iter != message.rend(); ++r_iter) {
		int count = (int)((float)r_iter->FullText().size() / (float)width);
		for (int i = 0; i <= count; ++i) {
			std::string subtext = r_iter->FullText().substr((count - i) * width, width);
			tcod::print(console, {x, y + y_offset}, subtext, r_iter->GetColor(), black);
			--y_offset;
			if (y_offset < 0) {
				return;
			}
		}

	}
}
