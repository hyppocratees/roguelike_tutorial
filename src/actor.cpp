#include "actor.h"

void Actor::TakeDamage(int damage) {
	fighter_.Hp(fighter_.GetHp() - damage);
	if (IsDead()) {
		Die();
	}
}