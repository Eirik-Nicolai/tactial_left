#include "entt/entt.hpp"
#include "logger.hpp"
#include "components/debugging.hpp"
#include "components/rendering.hpp"
#include "components/components.hpp"


// template<typename T>
// const char *GetTypeName();

// #define MAKE_TEMPLATE_NAME(T) \
//   template<> \
//   const char *GetTypeName<T>() { return #T; }

// struct Foo {};
// class Bar {};

// MAKE_TEMPLATE_NAME(Pos);
// MAKE_TEMPLATE_NAME(Size);
// MAKE_TEMPLATE_NAME(Foo);
// MAKE_TEMPLATE_NAME(Bar);

/// TODO use to serialize+save, unload and load registry
/// states for different game states
class GameRegistry
{

  public:
    GameRegistry() = default;
    ~GameRegistry() = default;

    // ------ ECS helper functions ------
  public:
    // ADDING COMPONENT
    template <typename C>
    void add_tag(entt::entity &e)
    {
        m_reg.emplace<C>(e);
    }
    
    template <typename C>
    void add_component(entt::entity &e)
    {
        Error("Attempting to add a component with no arguments to entity "
              << entity_name(e) << ", consider using 'add_tag'");
    }
    template <typename C>
    void unsafe_add_component(entt::entity &e)
    {
        add_component<C>(e);
    }    
    template <typename C, typename... Args>
    void add_component(entt::entity &e, Args &&...args)
    {
        if (has_component<C>(e)) {
            Error("Attempting to add duplicate component to entity " << entity_name(e));
        }
        m_reg.emplace<C>(e, std::forward<Args>(args)...);
    }
    template <typename C, typename... Args>
    void unsafe_add_component(entt::entity &e, Args &&...args)
    {
        m_reg.emplace<C>(e, std::forward<Args>(args)...);
    }

    // REMOVING COMPONENT
    template <typename C>
    void remove_component(entt::entity &e)
    {
        if (auto b = m_reg.try_get<C>(e); b != nullptr) {
            m_reg.erase<C>(e);
            return;
        }
        Error("Attempted to remove unassigned component from entity "
              << entity_name(e));
    }

    template <typename C>
    void unsafe_remove_component(entt::entity &e)
    {
        m_reg.erase<C>(e);
    }

    // RETRIEVING COMPONENT
    template <typename C>
    C *get_component(entt::entity &e)
    {
        return m_reg.try_get<C>(e);;
    }

    // TODO this doesnt work
    // entt will default it to the const func no matter what
    // and it will be immutable
    // template <typename C>
    // C &unsafe_get_component(entt::entity &e)
    // {
    //     return m_reg.get<C>(e);
    // }

    template <typename C>
    bool has_component(entt::entity &e)
    {
        return get_component<C>(e) != nullptr;
    }

    // ENTITY CREATON AND HANDLING
    entt::entity create_entity(const char *name)
    {
        auto e = m_reg.create();
        unsafe_add_component<Debugging::DebugName>(e, name);
        return e;
    }

    // OTHER HELPER THINGS
    std::string_view entity_name(entt::entity &e)
    {
        return get_component<Debugging::DebugName>(e)->name;
    }
    // TODO I would have like some way of forwarding creations
    // of views and groups to clean up the syntax a bit but
    // i'm not figuring out that template hell rn
    entt::registry& get() { return m_reg; }
  private:
    entt::registry m_reg;

    std::string get_name() const { return "GAME REGISTRY"; }
};
