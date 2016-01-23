#pragma once
#include <string>
#include "vmath.hpp"

// Represents a unit turret type
struct TurretType
{
    // The turret model ID.
    unsigned int turretModelId;

    // The physical offset of the turret on the body.
    vmath::vec3 translationOffset;
    vmath::quaternion rotationOffset;

    // Display name of the turret.
    std::string name;
};

// Represents a unit turret
struct Turret
{
    // The ID of the turret type.
    unsigned int turretTypeId;

    // The current translation and rotation of the turret, w. r. t. the default location.
    vmath::vec3 currentTranslation;
    vmath::quaternion currentRotation;
};
