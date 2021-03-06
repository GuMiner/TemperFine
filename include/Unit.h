#pragma once
#include <vector>
#include "ArmorInfo.h"
#include "BodyInfo.h"
#include "ModelManager.h"
#include "TurretInfo.h"
#include "RouteVisual.h"
#include "SharedExclusiveLock.h"
#include "Vec.h"

// Represents a physical unit.
class Unit
{
    public:
        Unit();

        // Creates a new unit, with full armor.
        Unit(unsigned int armorTypeId, unsigned int bodyTypeId, std::vector<unsigned int> turretTypeIds, const vec::vec3 position, const vec::quaternion rotation);
        
        // Performs rendering updates that need to be done in the physics thread that don't draw anything.
        void PerformGuiThreadUpdates(RouteVisual& routeVisual);

        // Renders the unit.
        void Render(ModelManager& modelManager, RouteVisual& unitRouter, bool isSelected, vec::mat4& projectionMatrix);

        // Returns true if the unit is currently in the path of the ray, false otherwise.
        bool InRayPath(const vec::vec3& rayStart, const vec::vec3& rayVector);

        // Updates (or adds) an assigned route for a unit.
        void UpdateAssignedRoute(std::vector<vec::vec3> newAssignedRoute);

        // Performs realistic unit motion.
        void MoveAlongRoute();

        // Moves the unit to the specified position.
        void Move(vec::vec3 pos);
        
    private:
        SharedExclusiveLock unitPhysicsLock;
        SharedExclusiveLock assignedRouteLock;

        // The current voxel the unit is above.
        vec::vec3i voxelPosition;

        // The physical location of the unit.
        vec::vec3 position;
        vec::quaternion rotation;

        // The route assigned to this unit.
        bool routeNeedsVisualUpdate;
        int routeVisualId;
        std::vector<vec::vec3> assignedRoute;
        unsigned int currentSegment;
        float currentSegmentPercentage;

        // Whether the unit will travel on the same route again after completing the route.
        bool routeLoops;

        // The turrets the unit has.
        std::vector<Turret> turrets;

        // The body the unit has.
        unsigned int bodyTypeId;

        // The armor applied to this unit.
        Armor armor;
};
