#include "combatstate.hpp"

#include "logger.hpp"

#include "utils/ecs.hpp"
#include "utils/geometry.hpp"
#include "utils/complex_datatypes.hpp"

#include "components/components.hpp"
#include "components/combat.hpp"
#include "components/interaction.hpp"
#include "components/rendering.hpp"

using namespace PlayingState;
// CombatState* CombatState::m_state;

class TestInput : public Input {
    public:
    std::string get_name() const final { return "Test Input StarState key P"; };
    void execute(TacticalGame *ge) final {
        // Error("Triggered input {}", get_name());
    }
};

class PanInputStart : public Input {
    public:
    std::string get_name() const final { return "InputPanningStart"; };
    void execute(TacticalGame *ge) final {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        ge->get_tv()->StartPan(pos_mouse);
    }
};

class ScrollUpInput : public Input {
    public:
    std::string get_name() const final { return "ScrollUpInputStart"; };
    void execute(TacticalGame *ge) final {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        tv->ZoomAtScreenPos(0.5f, pos_mouse);
    }
};

class ScrollDownInput : public Input {
    public:
    std::string get_name() const final { return "ScrollDownInputStart"; };
    void execute(TacticalGame *ge) final {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        tv->ZoomAtScreenPos(2.f, pos_mouse);
    }
};

class PanInputEnd : public Input {
    public:
    std::string get_name() const final { return "InputPanningEnd"; };
    void execute(TacticalGame *ge) final {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        ge->get_tv()->EndPan(pos_mouse);
    }
};

class PanInputUpdate : public Input {
    public:
    std::string get_name() const final { return "InputPanningUpdate"; };
    void execute(TacticalGame *ge) final {
        auto tv = ge->get_tv();
        auto pos_mouse = ge->GetMousePos();
        ge->get_tv()->UpdatePan(pos_mouse);
    }
};


CombatState::CombatState() {
    LOG_FUNC
    handler = std::make_unique<InputHandler>();

    handler->register_input(INPUT_TYPE::middle_mouse_PRESSED,
                            std::make_shared<InputDebugWrapper>(
                                std::make_shared<PanInputStart>()));
    handler->register_input(INPUT_TYPE::middle_mouse_RELEASED,
                            std::make_shared<InputDebugWrapper>(
                                std::make_shared<PanInputEnd>()));
    handler->register_input(INPUT_TYPE::middle_mouse_HELD,
                            std::make_shared<InputDebugWrapper>(
                                std::make_shared<PanInputUpdate>()));

    handler->register_input(INPUT_TYPE::middle_mouse_SCROLLDOWN,
                            std::make_shared<InputDebugWrapper>(
                                std::make_shared<ScrollUpInput>()));

    handler->register_input(INPUT_TYPE::middle_mouse_SCROLLUP,
                            std::make_shared<InputDebugWrapper>(
                                std::make_shared<ScrollDownInput>()));

    handler->register_input(INPUT_TYPE::left_mouse_PRESSED,
                            std::make_shared<TestInput>());
}
CombatState::~CombatState() {
    LOG_FUNC
}

void CombatState::pause(TacticalGame* ge) {
    LOG_FUNC

}
void CombatState::resume(TacticalGame* ge) {
    LOG_FUNC

}

void CombatState::enter(TacticalGame* ge) {
    LOG_FUNC

    Debug("Entering combat state");
    auto& reg = ge->get_reg();
    auto tv = ge->get_tv();

    auto sw = ge->ScreenWidth();
    auto sh = ge->ScreenHeight();
    auto w = (0.04);
    auto h = (0.05);
    auto rect_w = sw*w;
    auto rect_h = sh*h;
    auto offs_x = (sw/2) - (rect_w*tile_amt_x/2);
    auto offs_y = (sh/2) - (rect_h*tile_amt_y/2);

    Info("Node amt {}", combat_tiles.size());
    for(auto x = 0; x < tile_amt_x; x++)
        for(auto y = 0; y < tile_amt_y; y++) {

            auto tile = reg.create();
            reg.emplace<Interaction::_selectable>(tile, false);
            reg.emplace<Interaction::_hoverable>(tile, false);
            reg.emplace<Combat::Terrain::_path>(tile);

            Pos p;
            p.x=offs_x + (x*rect_w);
            p.y=offs_y + (y*rect_h);
            reg.emplace<Pos>(tile, p);
            reg.emplace<Size>(tile, (float)rect_w, (float)rect_h);
            combat_tiles[x + (tile_amt_x*y)] = std::make_shared<Node>(
                p.x,
                p.y
            );

            Rendering::Wireframe wire;
            wire.color = olc::DARK_RED;
            wire.type = Rendering::Wireframe::TYPE::SQUARE;
            reg.emplace<Rendering::Wireframe>(tile, wire);
        }

    for(auto x = 0; x < tile_amt_x; x ++)
        for(auto y = 0; y < tile_amt_y; y ++) {
            if(y>0)
                combat_tiles[x + (tile_amt_x*y)]->add_neighbour(combat_tiles[x + 0 + (tile_amt_x*(y - 1))]);
            if(y<tile_amt_y-1)
                combat_tiles[x + (tile_amt_x*y)]->add_neighbour(combat_tiles[x + 0 + (tile_amt_x*(y + 1))]);
            if(x>0)
                combat_tiles[x + (tile_amt_x*y)]->add_neighbour(combat_tiles[x - 1 + (tile_amt_x*(y - 0))]);
            if(x<tile_amt_x-1)
              combat_tiles[x + (tile_amt_x*y)]->add_neighbour(combat_tiles[x + 1 + (tile_amt_x*(y + 0))]);
        }

    node_start = combat_tiles[0];

    node_end = combat_tiles[3];

    solve_a_star();
}
void CombatState::exit(TacticalGame* ge) {
    LOG_FUNC

}

void CombatState::handle_input(TacticalGame* ge) {
    //LOG_FUNC
    // HACK FOR PANNING
    auto input = handler->get_input(ge);
    input->execute(ge);
}
void CombatState::update(TacticalGame* ge) {
    //LOG_FUN
    auto& reg = ge->get_reg();
    auto tv = ge->get_tv();
    auto pos_mouse = ge->GetMousePos();
    auto mouse_pos = tv->ScaleToWorld(pos_mouse) + tv->GetWorldOffset();

    // this should be moved to a separate system
    for(auto [ent, pos, size, hoverable] :
            reg.view<Pos, Size, Interaction::_hoverable>().each()) {
        if(is_point_inside_rect(pos, size, mouse_pos)) {
            hoverable.is_hovered=true;
        }
    }

    auto sw = ge->ScreenWidth();
    auto sh = ge->ScreenHeight();
    auto w = (0.04);
    auto h = (0.05);
    auto rect_w = sw*w;
    auto rect_h = sh*h;
    auto offs_x = (sw/2) - (rect_w*tile_amt_x/2);
    auto offs_y = (sh/2) - (rect_h*tile_amt_y/2);
    // // this should be moved to a separate system
    // for(auto [ent, pos, size, selectable, hoverable] :
    //         reg.view<Pos, Size, Interaction::_selectable,
    //         Interaction::_hoverable>().each()) {
    //     if(ge->GetMouse(MOUSE_LBUTTON).bReleased&& hoverable.is_hovered) {
    //         auto x_normalized = ((pos.x-offs_x)/rect_w);
    //         auto y_normalized = ((pos.y-offs_y)/rect_h);
    //         auto index_pos = x_normalized + (tile_amt_x*y_normalized);
    //         if(selectable.is_selected) {
    //             selectable.is_selected=false;
    //             combat_tiles[index_pos]->is_obstacle = false;
    //         } else {
    //             selectable.is_selected=true;
    //             combat_tiles[index_pos]->is_obstacle = true;
    //         }

    //         // solve_a_star();
    //     }
    //     if(ge->GetMouse(MOUSE_RBUTTON).bReleased && hoverable.is_hovered) {
        
    //         auto x_normalized = ((pos.x-offs_x)/rect_w);
    //         auto y_normalized = ((pos.y-offs_y)/rect_h);
    //         auto index_pos = x_normalized + (tile_amt_x*y_normalized);
    //         combat_tiles[index_pos]->is_obstacle = false;
    //         node_start = combat_tiles[index_pos];

    //         //solve_a_star();
    //     }

    //     if(ge->GetMouse(MOUSE_MBUTTON).bReleased&& hoverable.is_hovered) {
    //         auto x_normalized = ((pos.x-offs_x)/rect_w);
    //         auto y_normalized = ((pos.y-offs_y)/rect_h);
    //         auto index_pos = x_normalized + (tile_amt_x*y_normalized);
    //         if(combat_tiles[index_pos]->weight > 60)
    //             combat_tiles[index_pos]->weight = 0;
    //         else
    //             combat_tiles[index_pos]->weight+=30;

    //         //solve_a_star();
    //     }
    //     if(!is_point_inside_rect(pos, size, mouse_pos)) {
    //         hoverable.is_hovered=false;
    //     }
    // }

    if(ge->animation_tick()) solve_a_star();
}

void CombatState::draw(TacticalGame* ge) {
    //LOG_FUNC

    auto& reg = ge->get_reg();
    for(auto [ent, wire, hoverable] :
            reg.view<Rendering::Wireframe, Interaction::_hoverable>().each()) {
        if(hoverable.is_hovered) {
            wire.color = olc::RED;
            wire.type = Rendering::Wireframe::TYPE::SQUARE;
        } else {
            wire.color = olc::DARK_RED;
            wire.type = Rendering::Wireframe::TYPE::SQUARE;
        }
    }
    for(auto [ent, wire, _] :
            reg.view<Rendering::Wireframe, Interaction::_selectable>().each()) {
        wire.color = olc::DARK_GREY;
        wire.type = Rendering::Wireframe::TYPE::SQUARE;
    }

    //HACK debugging purposes
    auto sw = ge->ScreenWidth();
    auto sh = ge->ScreenHeight();
    auto w = (0.04);
    auto h = (0.05);
    auto rect_w = sw*w;
    auto rect_h = sh*h;
    auto tv = ge->get_tv();

    for(auto x = 0; x < tile_amt_x; x ++)
        for(auto y = 0; y < tile_amt_y; y ++) {
            auto curr_node = combat_tiles[x + (tile_amt_x*y)];
            
            if(curr_node->is_visited) {
                tv->FillRect(curr_node->x+4, curr_node->y+4, rect_w-4, rect_h-4, olc::VERY_DARK_RED);
            }
            if(curr_node->weight > 0) {
                tv->FillRect(curr_node->x+4, curr_node->y+4, rect_w-4, rect_h-4, olc::DARK_MAGENTA);
            }
            if(curr_node->weight > 30) {
                tv->FillRect(curr_node->x+4, curr_node->y+4, rect_w-4, rect_h-4, olc::MAGENTA);
            }
            if(curr_node->is_obstacle) {
                tv->FillRect(curr_node->x+4, curr_node->y+4, rect_w-4, rect_h-4, olc::WHITE);
            }
        }
  
    if(node_start)
        tv->FillRect(node_start->x+2, node_start->y+2, rect_w-2, rect_h-2, olc::BLUE);

    // draw line for neighbour connections
    for(auto x = 0; x < tile_amt_x; x ++)
        for(auto y = 0; y < tile_amt_y; y ++) {
            //Error("Drawing for {} {} ", x, y);
            auto curr_node = combat_tiles[x + (tile_amt_x*y)];
            for(auto i = 0; i < curr_node->neighbour_count; ++i) {
                auto neighbour = curr_node->neighbours[i];
                tv->DrawLine(curr_node->x + rect_w/2, curr_node->y + rect_h/2,
                             neighbour->x + rect_w/2, neighbour->y + rect_h/2, olc::WHITE, 0x0f0f0f0f);
            }
        }

    // draw path
    if(node_end) {
        tv->FillRect(node_end->x+2, node_end->y+2, rect_w-2, rect_h-2, olc::GREEN);

        auto n = node_end;
        while(n->parent) {
            tv->DrawLineDecal(olc::vf2d(n->x + rect_w/2, n->y + rect_h/2),
                              olc::vf2d(n->parent->x + rect_w/2, n->parent->y + rect_h/2),
                              olc::YELLOW);
            n = n->parent;
        }
    }
}

// TODO move to separate thread
void CombatState::solve_a_star() {
    for(auto x = 0; x < tile_amt_x; x ++)
        for(auto y = 0; y < tile_amt_y; y ++) {
            auto index = x + (tile_amt_x*y);
            combat_tiles[index]->is_visited = false;
            combat_tiles[index]->global_goal = INFINITY;
            combat_tiles[index]->local_goal = INFINITY;
            combat_tiles[index]->parent = nullptr;
        }

    auto distance = [](std::shared_ptr<Node> a, std::shared_ptr<Node> b)
    {
        return sqrtf((a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y));
    };

    auto heuristic = [distance](std::shared_ptr<Node> a, std::shared_ptr<Node> b)
    {
        return distance(a,b) + (a->weight + b->weight)*(a->weight + b->weight);
    };

    auto node_current = node_start;
    node_start->local_goal = 0.f;
    node_start->global_goal = heuristic(node_start, node_end);
    // list for sort func
    std::list<std::shared_ptr<Node>> not_tested_nodes;
    not_tested_nodes.push_back(node_start);
    while(!not_tested_nodes.empty())
    //while(!not_tested_nodes.empty() && node_current != node_end)
    {
      not_tested_nodes.sort([](std::shared_ptr<Node> lhs, std::shared_ptr<Node> rhs){
            return lhs->global_goal < rhs->global_goal;
        });

        while(!not_tested_nodes.empty() && not_tested_nodes.front()->is_visited)
            not_tested_nodes.pop_front();

        if(not_tested_nodes.empty())
            break;

        node_current = not_tested_nodes.front();
        node_current->is_visited = true;
        for(auto i = 0; i < node_current->neighbour_count; ++i) {
            auto neighbour = node_current->neighbours[i];
            if(!neighbour->is_visited && !neighbour->is_obstacle)
                not_tested_nodes.push_back(neighbour);
            float possible_local_goal = node_current->local_goal + distance(node_current, neighbour);

            if(possible_local_goal < neighbour->local_goal) {
                neighbour->parent = node_current;
                neighbour->local_goal = possible_local_goal;

                neighbour->global_goal = neighbour->local_goal + heuristic(neighbour, node_end);
            }
        }

    }
}
