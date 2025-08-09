#include "components/fighter.h"
#include "actor.h"

#include <format>
#include <iostream>
#include <string>

void Fighter::Die() {
	std::string death_message;
	if (entity_->GetName() == "player") {
		death_message = "You died!";
	}
	else {
		death_message = std::format("{} is dead!", entity_->GetName());
	}
	entity_->SetChar('%');
	entity_->SetBlockMove(false);
	entity_->SetAI(nullptr);
	entity_->SetName(std::format("remains of {}", entity_->GetName()));
	std::cout << death_message << std::endl;
}