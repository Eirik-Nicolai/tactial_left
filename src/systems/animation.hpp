#pragma once
#include "game.hpp"
#include "entt/entt.hpp"

#include "utils/debugging.hpp"
#include "systems/system.hpp"

class AnimationSystem : public System
{
    GET_NAME(System, AnimationSystem)
    virtual void execute(TacticalGame *ge) override {};
};

class CharacterAnimation : public AnimationSystem
{
    GET_NAME(AnimationSystem, CharacterAnimation)
    void execute(TacticalGame *ge) override;
};

// idk if we need this
class BGAnimation : public AnimationSystem
{
    GET_NAME(AnimationSystem, BGAnimation)
    void execute(TacticalGame *ge) override;
};

class GUIAnimation : public AnimationSystem
{
    GET_NAME(AnimationSystem, GUIAnimation)
    void execute(TacticalGame *ge) override;
};
