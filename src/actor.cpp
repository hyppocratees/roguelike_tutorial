#include "actor.h"
#include "components/ai.h"

void Actor::TakeDamage(int damage) {
	fighter_.Hp(fighter_.GetHp() - damage);
	if (IsDead()) {
		Die();
	}
}


std::ostream& operator<<(std::ostream& os, const Actor& ac) {
    os << ac.name_ << "\n";
    os << ac.char_ << "\n";
    os << ac.x_ << " " << ac.y_ << "\n";
    os << ac.block_mov_ << "\n";
    os << (*ac.ai_) << "\n";
    os << ac.fighter_;
    os << ac.inv_ << "\n";
    return os;
}

std::istream& operator>>(std::istream& is, Actor& ac) {
    //is >> ac.name_;
    is >> std::ws;
    std::getline(is, ac.name_);
    is >> ac.char_;
    is >> ac.x_ >> ac.y_;
    is >> ac.block_mov_;
    int ai_type;
    is >> ai_type;
    std::unique_ptr<BaseAI> ai;
    std::unique_ptr<BaseAI> prev_ai;
    int prev_ai_type;
    int remaining_turn;
    switch (ai_type) {
    case 0:
        ai = std::make_unique<BaseAI>(ac);
        ac.SetAI(ai);
        break;
    case 1:
        ai = std::make_unique<HostileAI>(ac);
        ac.SetAI(ai);
        break;
    case 2:
        is >> remaining_turn >> prev_ai_type;
        switch (prev_ai_type)
        {
        case 0:
            prev_ai = std::make_unique<BaseAI>(ac);
            break;
        case 1:
            prev_ai = std::make_unique<HostileAI>(ac);
            break;
        default:
            prev_ai = std::make_unique<BaseAI>(ac);
        }
        ai = std::make_unique<ConfusedAI>(ac, prev_ai, remaining_turn);
        ac.SetAI(ai);
        break;
    default:
        ac.SetAI(ai);
    }
    is >> ac.fighter_;
    is >> ac.inv_;
    return is;
}