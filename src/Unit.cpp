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
    for (int i = 0; i < turretTypeIds.size() && i < BodyConfig::Bodies[bodyTypeId].maxTurrets; i++)
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
    // TODO test code remove, renders a model using the model manager.
    vmath::mat4 identityMatrix = vmath::mat4::identity();
    modelManager.RenderModel(projectionMatrix, ArmorConfig::Armors[armor.armorTypeId].armorModelId, identityMatrix);

    vmath::mat4 translationMatrix = vmath::translate(200, 0, 0);
    modelManager.RenderModel(projectionMatrix, ArmorConfig::Armors[armor.armorTypeId].armorModelId, translationMatrix);

    translationMatrix = vmath::translate(100, 0, 0) * vmath::scale(0.1f, 0.1f, 0.1f);
    modelManager.RenderModel(projectionMatrix, TurretConfig::Turrets[turrets[0].turretTypeId].turretModelId, translationMatrix);

    translationMatrix = vmath::translate(-50, 20, 0) * translationMatrix;
    modelManager.RenderModel(projectionMatrix, BodyConfig::Bodies[bodyTypeId].bodyModelId, translationMatrix);
}
