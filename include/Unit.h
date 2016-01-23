#pragma once
#include <vector>
#include "ArmorInfo.h"
#include "BodyInfo.h"
#include "TurretInfo.h"
#include "vmath.hpp"

// Represents a physical unit.
class Unit
{
    public:
        Unit();
    protected:
    private:
        // The physical location of the unit.
        vmath::vec3 position;
        vmath::quaternion rotation;

        // The turrets the unit has.
        std::vector<Turret> turretIds;

        // The body the unit has.
        BodyType body;

        // The armor applied to this unit.
        Armor armor;
};
