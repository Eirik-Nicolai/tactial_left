#include "combatstate_playeraction.hpp"
using namespace PlayingState;

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
