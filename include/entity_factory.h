#pragma once
#include "actor.h"
#include "components/ai.h"

static const Actor PLAYER{ 0, 0, '@', {255, 255, 255}, "player", true, 1, Fighter(30, 2, 5)};
static const Actor ORC{ 0, 0, 'o', {63, 127, 63}, "orc", true, 1, Fighter(10, 0, 3)};
static const Actor TROLL{ 0, 0, 'T', {0, 127, 0}, "troll", true, 1, Fighter(16, 1, 4) };