#pragma once
#include "gamemap.h"

class Engine;
class Entity;

class Action {
public:
    void virtual Perform(Engine& engine, Entity& player) const = 0;
};

class EscapeAction : public Action {
public:
    void virtual Perform(Engine& engine, Entity& player) const;
};

class MovementAction : public Action {
public:
    MovementAction(int dx, int dy);
    void virtual Perform(Engine& engine, Entity& player) const;
    int getdx() const { return dx_; };
    int getdy() const { return dy_; };
private:
    int dx_;
    int dy_;
};