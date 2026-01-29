#pragma once
#include "game.hpp"
#include "entt/entt.hpp"

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

class Renderer : public System {
    std::string_view get_name() override { return "System - Renderer"; }
    virtual void execute(TacticalGame* ge) override {};
};

class PreRenderer : public Renderer {
    // TODO can we make this a macro to generate names of subclasses like this
    std::string_view get_name() override { return "System - Renderer - PreRenderer"; }
    void execute(TacticalGame* ge) override;
};

class FirstRenderer : public Renderer {
    // TODO can we make this a macro to generate names of subclasses like this
    std::string_view get_name() override { return "System - Renderer - FirstRenderer"; }
    void execute(TacticalGame* ge) override;
};

class SecondRenderer : public Renderer {
    // TODO can we make this a macro to generate names of subclasses like this
    std::string_view get_name() override { return "System - Renderer - SecondRenderer"; }
    void execute(TacticalGame* ge) override;
};

class ThirdRenderer : public Renderer {
    // TODO can we make this a macro to generate names of subclasses like this
    std::string_view get_name() override { return "System - Renderer - ThirdRenderer"; }
    void execute(TacticalGame* ge) override;
};

class PostRenderer : public Renderer {
    // TODO can we make this a macro to generate names of subclasses like this
    std::string_view get_name() override { return "System - Renderer - PostRenderer"; }
    void execute(TacticalGame* ge) override;
};

class GUIRenderer : public Renderer {
    // TODO can we make this a macro to generate names of subclasses like this
    std::string_view get_name() override { return "System - Renderer - GUIRenderer"; }
    void execute(TacticalGame* ge) override;
};


// Debugging
class WireframeRenderer : public Renderer {
    // TODO can we make this a macro to generate names of subclasses like this
    std::string_view get_name() override { return "System - Renderer - WireframeRenderer"; }
    void execute(TacticalGame* ge) override;
};


class RenderingSystemManager : public SystemManager {
  public:
    std::string_view get_name() override { return "RenderingSystemManager"; }
    RenderingSystemManager();
    void add(std::unique_ptr<System> system) override;
    void dispatch(TacticalGame* ge) override;
};
