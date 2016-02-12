#pragma once
#include <string>
#include "Vec.h"

// Represents a type of armor with graphical settings.
struct ArmorType
{
    // The armor model ID.
    unsigned int armorModelId;

    // Armor in-game constants.
    float maxMachineGun;
    float maxRocket;
    float maxCannon;

    // Armor is destroyed when the machine gun, rocket, and cannon has maxed out.
    // If a unit is only taking machine gun fire, and that maxes out, a penalty is
    //  applied for future machine gun fire as a multiplier to the damage.
    float maxMachineGunPenalty;
    float maxRocketPenalty;
    float maxCannonPenalty;

    // Physical offsets of the armor from the center of the unit for display.
    vec::vec3 translationOffset;
    vec::quaternion rotationOffset;

    // Display name of the armor.
    std::string name;
};

struct Armor
{
    // The type of armor this unit has.
    unsigned int armorTypeId;

    // The amount of machine gun fire the armor has taken.
    float machineGunDamage;

    // The amount of rocket fire the the armor has taken.
    float rocketDamage;

    // The amount of cannon fire the the armor has taken.
    float cannonDamage;
};
