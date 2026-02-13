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
    GET_NAME(CharacterAnimation, AnimationSystem)
    void execute(TacticalGame *ge) override;
};

// idk if we need this
class BGAnimation : public AnimationSystem
{
    GET_NAME(BGAnimation, AnimationSystem)
    void execute(TacticalGame *ge) override;
};

class GUIAnimation : public AnimationSystem
{
    GET_NAME(GUIAnimation, AnimationSystem)
    void execute(TacticalGame *ge) override;
};
