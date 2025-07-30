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
class ActionWithDirection : public Action {
public:
    ActionWithDirection(int dx, int dy);
    void virtual Perform(Engine& engine, Entity& player) const = 0;
    virtual int getdx() const { return dx_; };
    virtual int getdy() const { return dy_; };
protected:
    int dx_;
    int dy_;
};


class MovementAction : public ActionWithDirection {
public:
    MovementAction(int dx, int dy);
    void virtual Perform(Engine& engine, Entity& player) const;
};

class MeleeAction : public ActionWithDirection {
public:
    MeleeAction(int dx, int dy);
    void virtual Perform(Engine& engine, Entity& player) const;
};

class BumpAction : public ActionWithDirection {
public:
    BumpAction(int dx, int dy);
    void virtual Perform(Engine& engine, Entity& player) const;
};