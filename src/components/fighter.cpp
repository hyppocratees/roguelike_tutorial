#include "components/fighter.h"
#include "actor.h"

#include <format>
#include <iostream>
#include <string>

void Fighter::Die() {

	entity_->SetChar('%');
	entity_->SetBlockMove(false);
	entity_->SetAI(std::make_unique<DeadAI>(entity_));
	entity_->SetName(std::format("remains of {}", entity_->GetName()));
	entity_->SetRendOrd(0);

}