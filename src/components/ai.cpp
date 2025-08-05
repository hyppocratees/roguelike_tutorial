#include "components/ai.h"
#include "entity.h"
#include "gamemap.h"

#include <vector>
#include <utility>
#include <libtcod/path.hpp>

std::vector<std::pair<int, int>> BaseAI::GetPathTo(int dest_x, int dest_y)
{
	std::vector<std::pair<int, int>> result_path;
	TCODPath path(entity_.GetMap()->GetMap());
	path.compute(entity_.GetX(), entity_.GetY(), dest_x, dest_y);

	for (int i = 0; i < path.size(); ++i) {
		int* x;
		int* y;
		path.get(i, x, y);
		result_path.emplace_back(*x, *y);
	}
	return result_path;
}
