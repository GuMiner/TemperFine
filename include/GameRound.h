#pragma once
#include <vector>
#include "MapInfo.h"
#include "Player.h"

// Holds the subset of information required for a game round.
//  This is data that is heavily updated in the phyics thread and constantly displayed with the GUI. 
class GameRound
{
public:
    GameRound();

    // Players in the round
    std::vector<Player> players;

    // Current map in the round.
    MapInfo map;
};