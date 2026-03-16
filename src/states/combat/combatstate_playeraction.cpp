#include "combatstate_playeraction.hpp"
using namespace State::Playing::Combat;

CombatStatePlayerAction::CombatStatePlayerAction(TacticalGame *ge,
                                                 std::shared_ptr<GameRegistry> reg)
    : CombatState(ge, reg)
{

    // ABILITY
    // ability has a list of effects
    // affects has a list of modifiers and or debuffs/buffs
    // debuffs/buffs has a list of modifiers
    auto ability = m_registry->create_entity("My Ability");
    m_registry->add_component<Component::Combat::Ability::Cost>(ability,100);
    m_registry->add_component<Component::Combat::Ability::TargetingSingular>(ability, 1);
    
    std::array<entt::entity, 20> effects;
    // auto weakness = m_reg.create();
    // m_reg.emplace<_debuff>(weakness);
    // m_reg.emplace<visual>(weakness, "WEAKENED");
    // m_reg.emplace<tick>(weakness, 2);
    // m_reg.emplace<dmg_modifier>(weakness, (float)0.0);

    
    
    m_registry->add_component<Component::Combat::Ability::Does>(ability, std::move(effects));
    
    // auto bleed = m_reg.create();
    // m_reg.emplace<_debuff>(bleed);
    // m_reg.emplace<visual>(bleed, "BLEEDING");
    // m_reg.emplace<damage>(bleed, 10);
    // m_reg.emplace<tick>(bleed, 2);

    // auto bleed_attack = m_reg.create();
    // m_reg.emplace<visual>(bleed_attack, "Bleed atk");
    // m_reg.emplace<adds_debuff>(bleed_attack, bleed);

    // auto weakness_attack = m_reg.create();
    // m_reg.emplace<visual>(weakness_attack, "Weakness atk");
    // m_reg.emplace<adds_debuff>(weakness_attack, weakness);
};

// HACK for testing
void CombatStatePlayerAction::update()
{
    // move entity to selected spot, one line at a time
    using namespace Component;
}

void CombatStatePlayerAction::draw()
{
    using namespace Component;
    
    m_game->DrawStringDecal({100, 100}, "IN PLAYER ACTION STATE", olc::WHITE, {1, 4});
}
