#pragma once
#include "gamemap.h"
#include "entity.h"

class Engine;

class Action {
public:
    void virtual Perform(Engine& engine) const = 0;
};

class EscapeAction : public Action {
public:
    void virtual Perform(Engine& engine) const;
};
class ActionWithDirection : public Action {
public:
    ActionWithDirection(Entity& entity, int dx, int dy);
    void virtual Perform(Engine& engine) const = 0;
    virtual int getdx() const { return dx_; };
    virtual int getdy() const { return dy_; };
protected:
    int dx_;
    int dy_;
    Entity& entity_;
};


class MovementAction : public ActionWithDirection {
public:
    MovementAction(Entity& entity, int dx, int dy);
    void virtual Perform(Engine& engine) const;
};

class MeleeAction : public ActionWithDirection {
public:
    MeleeAction(Entity& entity, int dx, int dy);
    void virtual Perform(Engine& engine) const;
};

class BumpAction : public ActionWithDirection {
public:
    BumpAction(Entity& entity, int dx, int dy);
    void virtual Perform(Engine& engine) const;
};

class WaitAction : public Action {
public:
    WaitAction(Entity& entity) : entity_(entity) {}
    void virtual Perform(Engine& engine) const {};
private:
    Entity& entity_;
};