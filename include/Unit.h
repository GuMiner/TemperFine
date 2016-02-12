#pragma once
#include <vector>
#include "ArmorInfo.h"
#include "BodyInfo.h"
#include "ModelManager.h"
#include "TurretInfo.h"
#include "UnitRouter.h"
#include "Vec.h"

// Represents a physical unit.
class Unit
{
    public:
        Unit();

        // Creates a new unit, with full armor.
        void CreateNew(unsigned int armorTypeId, unsigned int bodyTypeId, std::vector<unsigned int> turretTypeIds, const vec::vec3 position, const vec::quaternion rotation);

        // Renders the unit.
        void Render(ModelManager& modelManager, UnitRouter& unitRouter, bool isSelected, vec::mat4& projectionMatrix);

        // Returns true if the unit is currently in the path of the ray, false otherwise.
        bool InRayPath(ModelManager& modelManager, const vec::vec3& rayStart, const vec::vec3& rayVector);

        // Updates (or adds) an assigned route for a unit.
        void UpdateAssignedRoute(std::vector<vec::vec3> newAssignedRoute);

        // Moves the unit to the specified position.
        void Move(vec::vec3 pos);

        // TODO test remove
        void TestAddRayPath(vec::vec3 start, vec::vec3 end);

    protected:
    private:
        // TODO test remove
        int testKnownRouteCount;
        std::vector<int> testRouteIds;
        std::vector<std::vector<vec::vec3>> testRoutes;

        // The current voxel the unit is above.
        vec::vec3i voxelPosition;

        // The physical location of the unit.
        vec::vec3 position;
        vec::quaternion rotation;

        // The route assigned to this unit.
        int routeVisualId;
        bool routeNeedsVisualUpdate;
        std::vector<vec::vec3> assignedRoute;

        // Whether the unit will travel on the same route again after completing the route.
        bool routeLoops;

        // The turrets the unit has.
        std::vector<Turret> turrets;

        // The body the unit has.
        unsigned int bodyTypeId;

        // The armor applied to this unit.
        Armor armor;
};
