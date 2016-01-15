#pragma once
#include <string>
#include <vector>
#include "vmath.hpp"

// TODO move each of these to their own header file once I have loading setup.

// Represents a unit turret.
struct Turret
{
    // The turret model ID.
    unsigned int turretModelId;

    // The physical position of the turret on the body.
    vmath::vec3 translationOffset;
    vmath::quaternion rotationOffset;

    // Display name of the turret.
    std::string name;
};

// Represents a unit body that holds turrets and armor.
struct Body
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
    vmath::vec3 translationOffset;
    vmath::quaternion rotationOffset;

    // Display name of the armor.
    std::string name;
};

struct Armor
{
    // The type of armor this unit has.
    unsigned int armorTypeId;

    // The amount of machine gun fire the armor has taken.
    float machineGunStrength;

    // The amount of rocket fire the the armor has taken.
    float rocketStrength;

    // The amount of cannon fire the the armor has taken.
    float cannonStrength;
};

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
        Body body;

        // The armor applied to this unit.
        Armor armor;
};
