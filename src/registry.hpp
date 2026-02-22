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

template<class T>
concept HasName = requires(T a) {
  {T::title()} -> std::same_as<std::string>;  
};

class GameRegistry
{

  public:
    GameRegistry() = default;
    ~GameRegistry() = default;

    // ------ ECS helper functions ------
  public:
    // ADDING COMPONENT
    template <HasName C>
    void add_tag(entt::entity &e)
    {
        // Trace("Unsafe adding " << C::title() << " to entity " << entity_name(e));
        m_reg.emplace<C>(e);
    }
    // template <HasName C>
    // void add_component(entt::entity &e)
    // {
    //     Error("Attempting to add a component with no arguments to entity "
    //           << entity_name(e) << ", consider using 'add_tag'");
    // }
    // template <HasName C>
    // void unsafe_add_component(entt::entity &e)
    // {
    //     // Trace("Unsafe adding " << C::title() << " to entity " << entity_name(e));
    //     add_component<C>(e);
    // }

    template <HasName C, typename... Args>
    void add_component(entt::entity &e, Args &&...args)
    {
        // Trace("Adding " << C::title() << " to entity " << entity_name(e));
        if (has_component<C>(e)) {
            Error("Attempting to add duplicate component " << C::title() << "  to entity "
                                                           << entity_name(e));
            return;
        }
        m_reg.emplace<C>(e, std::forward<Args>(args)...);
    }
    template <HasName C, typename... Args>
    void unsafe_add_component(entt::entity &e, Args &&...args)
    {
        // Trace("Unsafe adding " << C::title() << " to entity " << entity_name(e));
        m_reg.emplace<C>(e, std::forward<Args>(args)...);
    }

    // REMOVING COMPONENT
    template <HasName C>
    void remove_component(entt::entity &e)
    {
        //Trace("Removing " << C::title() << " to entity " << entity_name(e));
        if (auto b = m_reg.try_get<C>(e); b != nullptr) {
            m_reg.erase<C>(e);
            return;
        }
        Error("Attempted to remove unassigned " << C::title() << " from entity "
              << entity_name(e));
    }

    template <HasName C>
    void unsafe_remove_component(entt::entity &e)
    {
        //Trace("Unsafe removing " << C::title() << " to entity " << entity_name(e));
        m_reg.erase<C>(e);
    }

    template <HasName C>
    void remove_all_instances_of_tag()
    {
        //Trace("Removing all instances of " << C::title());
        for (auto [ent] : m_reg.view<C>().each()) {
            unsafe_remove_component<C>(ent);
        }
    }
    
    template <HasName C>
    void remove_all_instances_of_component()
    {
        //Trace("Removing all instances of " << C::title());
        for (auto [ent, _] : m_reg.view<C>().each()) {
            unsafe_remove_component<C>(ent);
        }
    }

    
    //                      //
    // RETRIEVING COMPONENT //
    //                      //
    template <HasName C>
    C *get_component(entt::entity &e)
    {
        return m_reg.try_get<C>(e);
    }

    
    // OBS OBS
    // these functions are for retrieving entitis of
    // solo component holder (ie. camera, player, etc)
    // they return the first entity found
    template <HasName C>
    entt::entity *get_entity_of_component()
    {
        for (auto [ent, _] : m_reg.view<C>().each()) {
            return ent;
        }
    }
    template <HasName C>
    entt::entity *get_entity_of_tag()
    {
        for (auto [ent] : m_reg.view<C>().each()) {
            return ent;
        }
    }

    // TODO this doesnt work
    // entt will default it to the const func no matter what
    // and it will be immutable
    // template <HasName C>
    // C &unsafe_get_component(entt::entity &e)
    // {
    //     return m_reg.get<C>(e);
    // }

    template <HasName C>
    bool has_component(entt::entity &e)
    {
        return get_component<C>(e) != nullptr;
    }

    // ENTITY CREATON AND HANDLING
    inline entt::entity create_entity(const char* n)
    {
        auto e = m_reg.create();
        add_component<Component::Debugging::DebugName>(e, n);
        return e;
    }

    // OTHER HELPER THINGS
    inline std::string_view entity_name(entt::entity &e)
    {
        return get_component<Component::Debugging::DebugName>(e)->name;
    }
    // TODO I would have like some way of forwarding creations
    // of views and groups to clean up the syntax a bit but
    // i'm not figuring out that template hell rn
    entt::registry& get() { return m_reg; }
  private:
    entt::registry m_reg;

    std::string get_name() const { return "GAME REGISTRY"; }
};
