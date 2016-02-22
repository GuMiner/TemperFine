#pragma once
#include <string>
#include <set>
#include <vector>
#include "ModelManager.h"
#include "RouteVisual.h"
#include "SharedExclusiveLock.h"
#include "TechProgress.h"
#include "Unit.h"
#include "Vec.h"

// Represents an in-game player.
class Player
{
    public:
        Player();
        Player(const std::string& name, int id);
        
        void AddUnit(const Unit& unit);

        // Performs updates on the GUI thread required for units.
        void PerformUnitGuiUpdates(RouteVisual& routeVisuals);

        // Renders the player's units.
        void RenderUnits(ModelManager& modelManager, RouteVisual& routeVisual, vec::mat4& projectionMatrix);

        // Checks if the given world ray intersects with a unit.
        // Returns the index of the unit if true, -1 if false.
        int CollisionCheck(vec::vec3 cameraPos, vec::vec3 worldRay);

        // Either adds or removed the specified unit from the set of selected units.
        void ToggleUnitSelection(int unitId);

        // Returns the players selected units. VALID FOR PHYSICS THREAD ONLY
        const std::set<int>& GetSelectedUnits() const;

        // Updates a unit's route to the new given route.
        void UpdateUnitRoute(int unitId, const std::vector<vec::vec3>& route);

        // Moves the player's units along their assigned routes.
        void MoveUnits();

        // Updates the current player's research progress.
        void UpdateResearchProgress(float lastElapsedTime);

    private:
        // ID of the player
        int id;

        // Display name of the player
        std::string name;

        // Units the player has under their control.
        SharedExclusiveLock playerUnitVectorMutex;
        std::vector<Unit> units;

        // The currently-selected units for the player.
        SharedExclusiveLock unitSelectionMutex; // STL isn't thread-safe, and we read this in the GUI, write in physics, as usual...
        std::set<int> selectedUnits;

        // Player technology progress.
        SharedExclusiveLock techProgressMutex;
        TechProgress techProgress;

        int currentlyResearchingTech;
        float elapsedResearchTimeSeconds;
};
