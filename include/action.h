#pragma once
#include "gamemap.h"
#include "actor.h"
#include "item.h"

#include <utility>

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
    ActionWithDirection(Actor& entity, int dx, int dy);
    void virtual Perform(Engine& engine) const = 0;
    virtual int getdx() const { return dx_; };
    virtual int getdy() const { return dy_; };
protected:
    int dx_;
    int dy_;
    Actor& entity_;
};


class MovementAction : public ActionWithDirection {
public:
    MovementAction(Actor& entity, int dx, int dy);
    void virtual Perform(Engine& engine) const;
};

class MeleeAction : public ActionWithDirection {
public:
    MeleeAction(Actor& entity, int dx, int dy);
    void virtual Perform(Engine& engine) const;
};

class BumpAction : public ActionWithDirection {
public:
    BumpAction(Actor& entity, int dx, int dy);
    void virtual Perform(Engine& engine) const;
};

class WaitAction : public Action {
public:
    WaitAction(Actor& entity) : entity_(entity) {}
    void virtual Perform(Engine& engine) const {};
private:
    Actor& entity_;
};

class DieAction : public Action {
public:
    DieAction(Actor& entity) : entity_(entity) {}
    void virtual Perform(Engine& engine) const;
private:
    Actor& entity_;
};

class ReturnToMainGame : public Action {
public:
    void virtual Perform(Engine& engine) const;
};

class ItemAction : public Action {
public:
    ItemAction(Actor& entity, Item& item) : actor_(entity), item_(item), target_xy_({0,0}) {};
    ItemAction(Actor& entity, Item& item, std::pair<int, int> target_xy) : actor_(entity), item_(item), target_xy_(target_xy) {};
    void virtual Perform(Engine& engine) const;
    Actor& GetActor() const { return actor_; };

private:
    Actor& actor_;
    Item& item_;
    std::pair<int, int> target_xy_;
};