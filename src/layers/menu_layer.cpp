#include "menu_layer.hpp"

#include "engine/mouse_event.hpp"
#include "utils/geometry.hpp"
#include "asset_manager.hpp"

#include "components/gui.hpp"
#include "components/rendering.hpp"
#include "components/interaction.hpp"

using GUI::_is_gui;

MenuLayer::MenuLayer(TacticalGame *ge, std::shared_ptr<GameRegistry> reg) : Layer(ge, reg)
{
    using namespace Component;
    auto button1 = reg->create_entity("Test Button");
    reg->add_component<GUI::Button>(button1, "DO ACTION",
                                    [](std::shared_ptr<GameRegistry> reg) {

                                    });
    
    // TODO get size of decal when loaded in
    // and set the scale proportionate to this
    //
    // Should also have a system around state -> texture
    // for buttons
    //
    // Tiles in combat should be considered buttons for this
    Rendering::TextureList tlist;
    tlist.amt_texture = 0;
    tlist.curr_texture = 0;
    tlist.textures[tlist.amt_texture++] = AssetManager::instance().load_texture(
        "assets/menuitems/Blue/ButtonsMiddle_[Normal]/Button-1.png",
        "Blue button normal");
    ;
    tlist.textures[tlist.amt_texture++] = AssetManager::instance().load_texture(
        "assets/menuitems/Blue/ButtonsMiddle_[Hover]/Button-1.png", "Blue button hover");
    ;
    tlist.textures[tlist.amt_texture++] = AssetManager::instance().load_texture(
        "assets/menuitems/Blue/ButtonsMiddle_[Disabled]/Button-1.png",
        "Blue button disable");
    reg->add_component<Rendering::TextureList>(button1, tlist);
    reg->add_tag<GUI::_is_gui>(button1);

    GUI::Pos p{100, 100};
    auto [button_w, button_h] = AssetManager::instance().get_texture_size(tlist.textures[0]);
    GUI::Size s{button_w, button_h};
    reg->add_component<GUI::Pos>(button1, p);
    reg->add_component<GUI::Size>(button1, s);

    reg->add_component<Interaction::Hoverable>(
        button1, Interaction::Hoverable{
                     .boundaries = Component::Box{Pos{p.x, p.y}, Size{s.w, s.h}},
                     .is_hovered = false,
                     // TODO later
                     .on_mouse_hover =
                         [this, ent = button1](std::shared_ptr<GameRegistry> reg, entt::entity me) {
                             auto get_name = []() { return "BUTTON"; };
                             Info("On hover !");
                             auto texturelist =
                                 m_registry->get_component<Rendering::TextureList>(ent);
                             texturelist->curr_texture = 1; 
                             return true;
                         },
                     .on_mouse_exit =
                         [this, ent = button1](std::shared_ptr<GameRegistry> reg, entt::entity me) {
                             auto get_name = []() { return "BUTTON"; };
                             Info("On exit !");
                             auto texturelist =
                                 m_registry->get_component<Rendering::TextureList>(ent);
                             texturelist->curr_texture = 0;
                             return true;
                         },
                 });
    Interaction::Selectable selectable;
    reg->add_component<Interaction::Selectable>(
        button1, [this](std::shared_ptr<GameRegistry> reg, entt::entity me) {
            auto get_name = [this, &reg, &me]() { return reg->entity_name(me); };
            Info("Selected !");
            return true;
    }, nullptr, nullptr);
}

MenuLayer::~MenuLayer() {}

void MenuLayer::on_event(Engine::Event &event)
{
    Engine::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Engine::MouseButtonReleasedEvent>(
        [this](Engine::MouseButtonReleasedEvent &e) {
            return mouse_button_released(e);
        });

    dispatcher.Dispatch<Engine::MouseButtonPressedEvent>(
        [this](Engine::MouseButtonPressedEvent &e) {
            return mouse_button_pressed(e);
        });
    
    dispatcher.Dispatch<Engine::MouseMovedEvent>(
        [this](Engine::MouseMovedEvent &e) {
            return mouse_moved(e);
        });
}

void MenuLayer::update() {
    
}
void MenuLayer::draw() {
    for (auto &&[ent, hoverable, _] :
         m_registry->get().view<Component::Interaction::Hoverable, GUI::Button>().each()) {
        // m_game->FillRect({hoverable.boundaries.pos.x, hoverable.boundaries.pos.y},
        //                  {hoverable.boundaries.size.w, hoverable.boundaries.size.h}, olc::BLUE);
    }
}

// TODO this can be centralized honestly, maybe all mouse moved events are called
// together whereas the mouse press events are not ?
bool MenuLayer::mouse_moved(Engine::MouseMovedEvent &event) {
    using namespace Component;
    using namespace Component;
    auto reg = m_game->registry();
    auto mouse_pos = m_game->GetMousePos();
    for (auto &&[ent, hoverable] :
         m_registry->get().view<Component::Interaction::Hoverable, _is_gui>().each()) {
        const bool was_hovered = hoverable.is_hovered;
        hoverable.is_hovered = is_point_inside_rect(
            hoverable.boundaries.pos, hoverable.boundaries.size, mouse_pos);
        m_game->DrawRect(
            {hoverable.boundaries.pos.x, hoverable.boundaries.pos.y},
            {hoverable.boundaries.size.w, hoverable.boundaries.size.h},
            hoverable.is_hovered ? olc::BLUE : olc::DARK_BLUE);

        // do action if state changed
        if ((!was_hovered && hoverable.is_hovered) && hoverable.on_mouse_hover) {
            return hoverable.on_mouse_hover(reg, ent);
        } else if ((was_hovered && !hoverable.is_hovered) && hoverable.on_mouse_exit) {
            return hoverable.on_mouse_exit(reg, ent);
        }
    }
    return false;
}

bool MenuLayer::mouse_button_released(Engine::MouseButtonReleasedEvent &event)
{
    for (auto &&[ent, hoverable, selectable] :
         m_registry->get().view<Component::Interaction::Hoverable, Component::Interaction::Selectable, GUI::_is_gui>().each()) {
        if(hoverable.is_hovered) {
            switch(event.get_button()) {
            case Engine::MouseButtonEvent::MouseButton::LeftMouseButton: {
                if(selectable.on_left_select) selectable.on_left_select(m_registry, ent);
            } break;
            case Engine::MouseButtonEvent::MouseButton::RightMouseButton: {
                if(selectable.on_left_select) selectable.on_right_select(m_registry, ent);
            } break;
            case Engine::MouseButtonEvent::MouseButton::MiddleMouseButton: {
                if(selectable.on_left_select) selectable.on_middle_select(m_registry, ent);
            } break;
            }
        }
    }
    
    return false;
}
bool MenuLayer::mouse_button_pressed(Engine::MouseButtonPressedEvent &event)
{
    return false;
}
