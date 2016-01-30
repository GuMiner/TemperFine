#pragma once
#include <string>
#include <vector>
#include "ModelManager.h"
#include "Unit.h"
#include "vmath.hpp"

// Represents an in-game player.
class Player
{
    public:
        Player();

        // Renders the player's units.
        void RenderUnits(ModelManager& modelManager, vmath::mat4& projectionMatrix);

        // Adds the unit to the units the player has under control.
        void AddUnit(const Unit unit);

    protected:
    private:

        // Display name of the player
        std::string name;

        // Units the player has under their control.
        std::vector<Unit> units;
};
