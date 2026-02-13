#pragma once
#include "game.hpp"
#include "entt/entt.hpp"

#include "utils/debugging.hpp"
#include "systems/system.hpp"

// TODO move systems to subdir of states probably
// except global ones if there are any ?

// namespace State {
// namespace Star {

// void render_stars(entt::registry &reg, TacticalGame* ge);
// void render_gui(entt::registry &reg, TacticalGame* ge);
// void apply_shaders(entt::registry &reg, TacticalGame* ge);

// };
// };

class RenderingSystem : public System
{
    GET_NAME(System, RenderingSystem)
    virtual void execute(TacticalGame *ge) override {};
};

class PreRenderer : public RenderingSystem
{

    GET_NAME(RenderingSystem, PreRenderer)
    void execute(TacticalGame *ge) override;
};

class BackgroundRenderer : public RenderingSystem
{

    GET_NAME(RenderingSystem, FirstRenderer)
    void execute(TacticalGame *ge) override;
};

class MainRenderer : public RenderingSystem
{

    GET_NAME(RenderingSystem, SecondRenderer)
    void execute(TacticalGame *ge) override;
};

class PostRenderer : public RenderingSystem
{

    GET_NAME(RenderingSystem, PostRenderer)
    void execute(TacticalGame *ge) override;
};

class GUIRenderer : public RenderingSystem
{

    GET_NAME(RenderingSystem, GUIRenderer)
    void execute(TacticalGame *ge) override;
};

// Debugging
class WireframeRenderer : public RenderingSystem
{

    GET_NAME(RenderingSystem, WireframeRenderer)
    void execute(TacticalGame *ge) override;
};
