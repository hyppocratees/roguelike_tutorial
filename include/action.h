#pragma once
#include "entity.h"

class Action {
public:
    void virtual Execute(Entity& player) const = 0;
};

class EscapeAction : public Action {
public:
    void virtual Execute(Entity& player) const {};
};

class MovementAction : public Action {
public:
    MovementAction(int dx, int dy);
    void virtual Execute(Entity& player) const;
    int getdx() const { return dx_; };
    int getdy() const { return dy_; };
private:
    int dx_;
    int dy_;
};