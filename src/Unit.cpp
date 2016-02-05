#include "ArmorConfig.h"
#include "BodyConfig.h"
#include "TurretConfig.h"
#include "Unit.h"

Unit::Unit()
{
    //ctor
}

// Creates a new unit, with full armor.
// Note that the unrotated unit is pointing in the x direction, with zero rotation.
void Unit::CreateNew(unsigned int armorTypeId, unsigned int bodyTypeId, std::vector<unsigned int> turretTypeIds, const vmath::vec3 position, const vmath::quaternion rotation)
{
    this->position = position;
    this->rotation = rotation;

    // Armor has taken no damage.
    armor.armorTypeId = armorTypeId;
    armor.rocketDamage = 0.0f;
    armor.cannonDamage = 0.0f;
    armor.machineGunDamage = 0.0f;

    this->bodyTypeId = bodyTypeId;

    // Note that if the user provides more turrets than this body can hold, we truncate and only take the top ones listed.
    turrets.clear();
    for (unsigned int i = 0; i < turretTypeIds.size() && i < BodyConfig::Bodies[bodyTypeId].maxTurrets; i++)
    {
        Turret turret;
        turret.turretTypeId = turretTypeIds[i];
        turret.currentTranslation = vmath::vec3(0.0f, 0.0f, 0.0f);
        turret.currentRotation = vmath::quaternion::fromAxisAngle(0.0f, vmath::vec3(1, 0, 0));

        turrets.push_back(turret);
    }
}

void Unit::Render(ModelManager& modelManager, vmath::mat4& projectionMatrix)
{
    // We do a bunch of matrix math (but nothing to complex) to properly draw armor, bodies, and turrets.
    vmath::mat4 unitOrientation = vmath::translate(position) * rotation.asMatrix();

    const BodyType& bodyType = BodyConfig::Bodies[bodyTypeId];

    vmath::mat4 bodyMatrix = unitOrientation * vmath::scale(bodyType.scale, bodyType.scale, bodyType.scale);
    modelManager.RenderModel(projectionMatrix, bodyType.bodyModelId, bodyMatrix);

    const ArmorType& armorType = ArmorConfig::Armors[armor.armorTypeId];

    vmath::mat4 armorMatrix = vmath::translate(armorType.translationOffset) * bodyMatrix * armorType.rotationOffset.asMatrix();
    modelManager.RenderModel(projectionMatrix, armorType.armorModelId, armorMatrix);

    for (unsigned int i = 0; i < turrets.size(); i++)
    {
        const TurretType& turretType = TurretConfig::Turrets[turrets[i].turretTypeId];

        vmath::mat4 turretDefaultMatrix = vmath::translate(turretType.translationOffset) * bodyMatrix * turretType.rotationOffset.asMatrix();
        vmath::mat4 turretMatrix = vmath::translate(turrets[i].currentTranslation) * turretDefaultMatrix * turrets[i].currentRotation.asMatrix();
        modelManager.RenderModel(projectionMatrix, turretType.turretModelId, turretMatrix);
    }
}

void Unit::Move(vmath::vec3 pos)
{
    position = pos;
}
