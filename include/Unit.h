#pragma once
#include <vector>
#include "ArmorInfo.h"
#include "BodyInfo.h"
#include "ModelManager.h"
#include "TurretInfo.h"
#include "UnitRouter.h"
#include "vmath.hpp"

// Represents a physical unit.
class Unit
{
    public:
        Unit();

        // Creates a new unit, with full armor.
        void CreateNew(unsigned int armorTypeId, unsigned int bodyTypeId, std::vector<unsigned int> turretTypeIds, const vmath::vec3 position, const vmath::quaternion rotation);

        // Renders the unit.
        void Render(ModelManager& modelManager, UnitRouter& unitRouter, bool isSelected, vmath::mat4& projectionMatrix);

        // Returns true if the unit is currently in the path of the ray, false otherwise.
        bool InRayPath(ModelManager& modelManager, const vmath::vec3& rayStart, const vmath::vec3& rayVector);

        // Updates (or adds) an assigned route for a unit.
        void UpdateAssignedRoute(std::vector<vmath::vec3> newAssignedRoute);

        // Moves the unit to the specified position.
        void Move(vmath::vec3 pos);

    protected:
    private:
        // The current voxel the unit is above.
        vmath::vec3i voxelPosition;

        // The physical location of the unit.
        vmath::vec3 position;
        vmath::quaternion rotation;

        // The route assigned to this unit.
        int routeVisualId;
        bool routeNeedsVisualUpdate;
        std::vector<vmath::vec3> assignedRoute;

        // Whether the unit will travel on the same route again after completing the route.
        bool routeLoops;

        // The turrets the unit has.
        std::vector<Turret> turrets;

        // The body the unit has.
        unsigned int bodyTypeId;

        // The armor applied to this unit.
        Armor armor;
};
