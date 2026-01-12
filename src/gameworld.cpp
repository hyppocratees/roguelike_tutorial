#include "gameworld.h"
#include "gamemap.h"
#include "procgen.h"

void GameWorld::GenerateFloor(GameMap& map)
{
	current_floor_++;
	pmap_gen_->Generate(map);
}
