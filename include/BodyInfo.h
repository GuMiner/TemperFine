#pragma once
#include <string>

// Represents a unit body that holds turrets and armor.
struct BodyType
{
    // The body model ID.
    unsigned int bodyModelId;

    // The maximum number of turrets this body supports.
    unsigned int maxTurrets;

    // Scale of the body. Also scales up armor and turrets appropriately.
    //   Note that bodies are always centered at the unit position and at the unit rotation.
    //   Turret rotations are applied with respect to the unit rotation.
    float scale;

    // Display name of the body.
    std::string name;
};
