#pragma once
#include <vector>
#include "ArmorInfo.h"
#include "BodyInfo.h"
#include "ModelManager.h"
#include "TurretInfo.h"
#include "vmath.hpp"

// Represents a physical unit.
class Unit
{
    public:
        Unit();

        // Creates a new unit, with full armor.
        void CreateNew(unsigned int armorTypeId, unsigned int bodyTypeId, std::vector<unsigned int> turretTypeIds, const vmath::vec3 position, const vmath::quaternion rotation);

        // Renders the unit.
        void Render(ModelManager& modelManager, vmath::mat4& projectionMatrix);
    protected:
    private:
        // The physical location of the unit.
        vmath::vec3 position;
        vmath::quaternion rotation;

        // The turrets the unit has.
        std::vector<Turret> turrets;

        // The body the unit has.
        unsigned int bodyTypeId;

        // The armor applied to this unit.
        Armor armor;
};
