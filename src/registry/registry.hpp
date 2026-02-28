#include "entt/entt.hpp"
#include "logger.hpp"
#include "components/debugging.hpp"
#include "components/rendering.hpp"
#include "components/components.hpp"

/// TODO use to serialize+save, unload and load registry
/// states for different game states

template<class T>
concept HasName = requires(T a) {
  {T::title()} -> std::same_as<std::string>;  
};

class GameRegistry
{
    class GameEventDispatcher
    {
      public:
        GameEventDispatcher() = default;
        ~GameEventDispatcher() = default;

        //                  //
        // EVENT DISPATCHER //
        //                  //
        template <typename E, auto func, typename... Args>
        inline void add_listener_to_event(Args &&...value_or_instance)
        {
            m_dispatcher.sink<E>().template connect<func>(std::forward<Args>(value_or_instance)...);
        }

        // template <typename E, auto func, typename... Args>
        // inline void add_listener_to_event(Args &&...value_or_instance)
        // {
        //     m_dispatcher.sink<E>().template connect<func>(std::forward<Args>(value_or_instance)...);
        // }
        template <typename Event>
        inline void remove_all_listeners_on_event()
        {
            m_dispatcher.sink<Event>().template disconnect<>();
        }

        // template <typename Event, typename... Args>
        // inline void queue_event(Args &&...args)
        // {
        //     m_dispatcher.enqueue(Event{std::forward<Args>(args)...});
        // }

        template <typename Event>
        inline void queue_event(Event e)
        {
            m_dispatcher.enqueue(e);
        }
        
        template <typename Event>
        inline void queue_event()
        {
            m_dispatcher.enqueue<Event>();
        }
        template <typename Event, typename... Args>
        inline void immediate_dispatch(Args &&...args)
        {
            m_dispatcher.trigger(Event{std::forward<Args>(args)...});
        }

        template <typename Event>
        inline void immediate_dispatch()
        {
            m_dispatcher.trigger<Event>();
        }

        // dispatch all enqueued Event events
        template <typename Event>
        inline  void trigger_event()
        {
            m_dispatcher.update<Event>();
        }
        
        // dispatch all enqueued Event events with ARGS
        template <typename Event, typename... Args>
        inline void trigger_event(Args ...args)
        {
            m_dispatcher.update(Event{std::forward<Args>(args)...});
        }

        // dispatch all enqueued events
        inline void update()
        {
            m_dispatcher.update();
        }

        inline entt::dispatcher& get() {
            return m_dispatcher;
        }

      private:
        entt::dispatcher m_dispatcher;
        std::string get_name() const { return "GAME REGISTRY - Game Event Dispatcher"; }
    };

  public:
    GameRegistry() = default;
    ~GameRegistry() = default;

    // ------ ECS helper functions ------
  public:
    //                  //
    // ADDING COMPONENT //
    //                  //
    template <HasName C>
    void add_tag(entt::entity e)
    {
        Trace("Unsafe adding " << C::title() << " to entity " << entity_name(e));
        if(e==entt::null) return;
        m_reg.emplace<C>(e);
    }
    // template <HasName C>
    // void add_component(entt::entity e)
    // {
    //     Error("Attempting to add a component with no arguments to entity "
    //           << entity_name(e) << ", consider using 'add_tag'");
    // }
    // template <HasName C>
    // void unsafe_add_component(entt::entity e)
    // {
    //     // Trace("Unsafe adding " << C::title() << " to entity " << entity_name(e));
    //     add_component<C>(e);
    // }

    template <HasName C, typename... Args>
    void add_component(entt::entity e, Args &&...args)
    {
        Trace("Adding " << C::title() << " to entity " << entity_name(e));
        if(e==entt::null) return;
        if (has_component<C>(e)) {
            Error("Attempting to add duplicate component " << C::title() << "  to entity "
                                                           << entity_name(e));
            return;
        }
        m_reg.emplace<C>(e, std::forward<Args>(args)...);
    }
    template <HasName C, typename... Args>
    void unsafe_add_component(entt::entity e, Args &&...args)
    {
        Trace("Unsafe adding " << C::title() << " to entity " << entity_name(e));
        m_reg.emplace<C>(e, std::forward<Args>(args)...);
    }

    
    template <HasName C, typename... Args>
    void set_or_add_component(entt::entity e, Args &&...args)
    {
        Trace("Set/add component " << C::title() << " from entity " << entity_name(e));
        if(e==entt::null) return;
        return m_reg.emplace_or_replace<C>(e, std::forward<Args>(args) ...);
    }

    // template <HasName C, typename... Args>
    // C *unsafe_set_component(entt::entity e, Args &&...args)
    // {
    //     if(e==entt::null) return nullptr;
    //     return m_reg.replace<C>(e, std::forward<Args>(args) ...);
    // }
    
    template <HasName C>
    void unsafe_set_component(entt::entity e, std::function<void(C& c)> &&callback)
    {
        m_reg.patch<C>(e, std::forward<std::function<void(C& c)>>(callback));
    }
    //                    //
    // REMOVING COMPONENT //
    //                    //
    template <HasName C>
    void remove_component(entt::entity e)
    {
        Trace("Removing " << C::title() << " from entity " << entity_name(e));
        if(e==entt::null) return;
        m_reg.remove<C>(e);
    }

    template <HasName C>
    void unsafe_remove_component(entt::entity e)
    {
        Trace("Unsafe removing " << C::title() << " from entity " << entity_name(e));
        m_reg.erase<C>(e);
    }

    template <HasName C>
    void remove_all_instances_of_tag()
    {
        // TODO think theres an entt func for this
        Trace("Removing all instances of " << C::title());
        m_reg.clear<C>();
    }
    
    template <HasName C>
    void remove_all_instances_of_component()
    {
        // TODO think theres an entt func for this
        Trace("Removing all instances of " << C::title());
        m_reg.clear<C>();
    }
    
    //                  //
    // WORLD COMPONENTS //
    //                  //
    template <HasName C, typename... Args>
    C &set_world_component(Args ...args)
    {
        return m_reg.ctx().emplace<C>(std::forward<Args>(args)...);
    }
    
    template <HasName C>
    C &set_world_tag()
    {
        return m_reg.ctx().emplace<C>();
    }
    
    template <HasName C>
    bool get_world_component(C& component)
    {
        if(m_reg.ctx().contains<C>()) {
            component = m_reg.ctx().get<C>();
            return true;
        }
        return false;
    }
    
    template <HasName C>
    C &unsafe_get_world_component()
    {
        return m_reg.ctx().get<C>();
    }

    template <HasName C>
    void remove_world_component()
    {
        if (!m_reg.ctx().erase<C>()) {
            Warn("Attempted to remove world component " << C::title()
                                                        << " which did not exist");
        }
    }

    //                      //
    // RETRIEVING COMPONENT //
    //     AND ENTITIES     //
    //                      //
    template <HasName C>
    C *get_component(entt::entity e)
    {
        if(e==entt::null) return nullptr;
        return m_reg.try_get<C>(e);
    }

    template <HasName C>
    C *get_component_on_check(std::function<bool(C)> check)
    {
        for (auto [ent, c] : m_reg.view<C>().each()) {
            if(check(c))
                return c;
        }
        return entt::null;
    }

    template <HasName C>
    entt::entity get_entity_on_check(std::function<bool(C)> check)
    {
        for (auto [ent, c] : m_reg.view<C>().each()) {
            if(check(c))
                return ent;
        }
        return entt::null;
    }
        
    // OBS OBS
    // these functions are for retrieving entitis of
    // solo component holder (ie. camera, player, etc)
    // they return the first entity found
    template <HasName C>
    entt::entity get_single_entity_with_component()
    {
        for (auto [ent, _] : m_reg.view<C>().each()) {
            return ent;
        }
        return entt::null;
    }
    template <HasName C>
    entt::entity get_single_entity_with_tag()
    {
        for (auto [ent] : m_reg.view<C>().each()) {
            return ent;
        }
        return entt::null;
    }

    template <HasName C>
    const C &unsafe_get_component(entt::entity e)
    {
        return m_reg.get<C>(e);
    }

    template <HasName C>
    bool has_component(entt::entity e)
    {
        if(e==entt::null) return false;
        return get_component<C>(e) != nullptr;
    }

    //                             //
    // ENTITY CREATON AND HANDLING //
    //                             //
    inline entt::entity create_entity(const char* n)
    {
        auto e = m_reg.create();
        m_reg.emplace<Component::Debugging::DebugName>(e, n);
        return e;
    }

    //                  //
    // EVENT DISPATCHER //
    //                  //
    GameEventDispatcher dispatcher;
    
    //                     //
    // OTHER HELPER THINGS //
    //                     //
    inline std::string_view entity_name(entt::entity e)
    {
        if(e==entt::null) return "NULL_ENT";
        return m_reg.get<Component::Debugging::DebugName>(e).name;
    }
    // TODO I would have like some way of forwarding creations
    // of views and groups to clean up the syntax a bit but
    // i'm not figuring out that template hell rn
    entt::registry& get() { return m_reg; }

    // call update on all internals that need it
    void update(float deltatime)
    {
        std::ignore = deltatime; // not needed for the time being
        dispatcher.update();
    };
  private:
    entt::registry m_reg;
    std::string get_name() const { return "GAME REGISTRY"; }
};
