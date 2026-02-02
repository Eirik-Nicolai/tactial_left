#pragma once
#include "game.hpp"
#include "entt/entt.hpp"

#include "utils/debugging.hpp"
#include "systems/system.hpp"

class Animation : public System {
    GET_NAME(System, Animation)
    virtual void execute(TacticalGame* ge) override {};
};

class CharacterAnimation : public Animation {
    GET_NAME(CharacterAnimation, Animation)
    void execute(TacticalGame* ge) override;
};

//idk if we need this
class BGAnimation : public Animation {
    GET_NAME(BGAnimation, Animation)
    void execute(TacticalGame* ge) override;
};

class GUIAnimation : public Animation {
    GET_NAME(GUIAnimation, Animation)
    void execute(TacticalGame* ge) override;
};


