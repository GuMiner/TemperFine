#pragma once
#include <string>
#include <set>
#include <vector>
#include <SFML\System.hpp>
#include "ModelManager.h"
#include "Unit.h"
#include "RouteVisual.h"
#include "Vec.h"

// Represents an in-game player.
class Player
{
    public:
        Player();
        Player(const Player& player);
        Player(const std::string& name, int id);

        sf::Mutex playerUnitMutex;

        void AddUnit(const Unit unit);

        // Renders the player's units.
        void RenderUnits(ModelManager& modelManager, RouteVisual& routeVisual, vec::mat4& projectionMatrix);

        // Checks if the given world ray intersects with a unit.
        // Returns the index of the unit if true, -1 if false.
        int CollisionCheck(ModelManager& modelManager, vec::vec3 cameraPos, vec::vec3 worldRay);

        // Either adds or removed the specified unit from the set of selected units.
        void ToggleUnitSelection(int unitId);

        // Returns the players selected units.
        const std::set<int>& GetSelectedUnits() const;

        // Updates a unit's route to the new given route.
        void UpdateUnitRoute(int unitId, const std::vector<vec::vec3>& route);

        // Moves the player's units along their assigned routes.
        void MoveUnits();

    private:
        // ID of the player
        int id;

        // Display name of the player
        std::string name;

        // Units the player has under their control.
        std::vector<Unit> units;

        // The currently-selected units for the player.
        std::set<int> selectedUnits;
};
