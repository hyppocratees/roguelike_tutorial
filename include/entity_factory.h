#pragma once
#include "actor.h"
#include "item.h"
#include "components/consumable.h"
#include "components/ai.h"
#include "inventory.h"

static const Actor PLAYER{ 0, 0, '@', {255, 255, 255}, "player", true, 1, Fighter(30, 2, 5), 10, Inventory(26)};
static const Actor ORC{ 0, 0, 'o', {63, 127, 63}, "orc", true, 1, Fighter(10, 0, 3), 10, Inventory(0)};
static const Actor TROLL{ 0, 0, 'T', {0, 127, 0}, "troll", true, 1, Fighter(16, 1, 4), 10, Inventory(0)};
static const Item HEALTH_POTION{ 0, 0, '!', {127, 0, 255}, "health potion", false, 9, std::make_unique<HealingConsumable>(4)};
