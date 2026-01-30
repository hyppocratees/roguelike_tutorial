#pragma once
#include "actor.h"
#include "item.h"
#include "components/consumable.h"
#include "components/ai.h"
#include "inventory.h"
#include "equipment_types.h"

static const Actor PLAYER{ 0, 0, '@', {255, 255, 255}, "player", true, 1, Fighter(30, 2, 5), 10, Inventory(26), Level(nullptr, 1, 0, 200, 150, 0) };
static const Actor ORC{ 0, 0, 'o', {63, 127, 63}, "orc", true, 1, Fighter(10, 0, 3), 10, Inventory(0), Level(nullptr, 1, 0, 0, 1, 35) };
static const Actor TROLL{ 0, 0, 'T', {0, 127, 0}, "troll", true, 1, Fighter(16, 1, 4), 10, Inventory(0), Level(nullptr, 1, 0, 0, 1, 100) };
static const Item HEALTH_POTION{ 0, 0, '!', {127, 0, 255}, "health potion", false, 9, std::make_unique<HealingConsumable>(4)};
static const Item LIGHTNING_SCROLL{ 0, 0, '~', {255, 255, 0}, "lightning scroll", false, 9, std::make_unique<LightningDamageConsumable>(20, 5) };
static const Item CONFUSION_SCROLL{ 0,0, '~', {207, 63, 255}, "confusion scroll", false, 9, std::make_unique<ConfusionConsumable>(10) };
static const Item FIREBALL_SCROLL{ 0, 0, '~', {255, 0, 0}, "fireball scroll", false, 9, std::make_unique<FireballConsumable>(12, 3) };
static const Item DAGGER{ 0, 0, '/', {0, 191, 255}, "dagger", false, 9, std::make_unique<Equipable>(DAGGER_EQUIP) };
static const Item SWORD{ 0, 0, '/', {0, 191, 255}, "short sword", false, 9, std::make_unique<Equipable>(SWORD_EQUIP) };
static const Item LEATHER_ARMOR{ 0, 0, '/', {0, 191, 255}, "leather armor", false, 9, std::make_unique<Equipable>(LEATHER_ARMOR_EQUIP) };
static const Item CHAIN_MAIL{ 0, 0, '/', {0, 191, 255}, "chain mail", false, 9, std::make_unique<Equipable>(CHAIN_MAIL_EQUIP) };