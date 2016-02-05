#pragma once
#include <string>
#include <set>
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

        // Checks if the given world ray intersects with a unit.
        // Returns the index of the unit if true, -1 if false.
        int CollisionCheck(ModelManager& modelManager, vmath::vec3 cameraPos, vmath::vec3 worldRay);

        // Either adds or removed the specified unit from the set of selected units.
        void ToggleUnitSelection(int unitId);

        // Adds the unit to the units the player has under control.
        void AddUnit(const Unit unit);

    protected:
    private:

        // Display name of the player
        std::string name;

        // Units the player has under their control.
        std::vector<Unit> units;

        // The currently-selected units for the player.
        std::set<int> selectedUnits;
};
