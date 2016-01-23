#pragma once
#include <string>
#include <vector>
#include "Unit.h"
#include "vmath.hpp"

// Represents an in-game player.
class Player
{
    public:
        Player();
        void RenderUnits(vmath::mat4& projectionMatrix);

        ~Player();
    protected:
    private:

        // Display name of the player
        std::string name;

        // Units the player has under their control.
        std::vector<Unit> units;
};
