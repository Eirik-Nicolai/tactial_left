#pragma once
#include <functional>
#include <list>
#include <memory>
#include <string_view>
#include <utility>

// Linked list of checks
// able to check several things by running a node.check() with f.ex. a component
template <typename Type>
class CheckerNode {
  public:
    std::string_view get_name() const { return name; } // debugging
    CheckerNode(CheckerNode* next_node_, std::function<bool(Type)> condition_,
                std::string_view name_ = "Generic checker")
      : next_node(std::move(next_node_)), condition(condition_), name(name_)  {}

    inline bool check() {
      if(condition()) {
        if(next_node) next_node->check();
        else return true;
      }
      return false;
    }
  private:
    std::function<bool(Type)> condition;
    CheckerNode<Type>* next_node;
    std::string_view name;
};


struct Node {
  Node(int x_, int y_) : x(x_), y(y_) {
    is_obstacle = false;
    is_visited = false;
    global_goal = false;
    local_goal = false;
    parent = nullptr;
    weight = 0;
    neighbour_count = 0;
  }
  void add_neighbour(std::shared_ptr<Node> neighbour) {
    if(neighbour_count<neighbours.size())
      neighbours[neighbour_count++] = neighbour;
  }
  bool is_obstacle;
  bool is_visited;
  float global_goal;
  float local_goal;
  int x;
  int y;
  size_t neighbour_count;
  std::array<std::shared_ptr<Node>, 8> neighbours;
  std::shared_ptr<Node> parent;

  unsigned weight;
};

// inline
