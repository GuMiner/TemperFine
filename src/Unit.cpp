#include "ArmorConfig.h"
#include "BodyConfig.h"
#include "MatrixOps.h"
#include "TurretConfig.h"
#include "Unit.h"

Unit::Unit()
{
    routeVisualId = -1;
    routeNeedsVisualUpdate = false;
}

// Creates a new unit, with full armor.
// Note that the unrotated unit is pointing in the x direction, with zero rotation.
void Unit::CreateNew(unsigned int armorTypeId, unsigned int bodyTypeId, std::vector<unsigned int> turretTypeIds, const vec::vec3 position, const vec::quaternion rotation)
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
        turret.currentTranslation = vec::vec3(0.0f, 0.0f, 0.0f);
        turret.currentRotation = vec::quaternion::fromAxisAngle(0.0f, vec::vec3(1, 0, 0));

        turrets.push_back(turret);
    }
}

void Unit::Render(ModelManager& modelManager, UnitRouter& unitRouter, bool isSelected, vec::mat4& projectionMatrix)
{
    // Update the route visual if the physics system has recomputed it.
    if (routeNeedsVisualUpdate)
    {
        if (routeVisualId != -1)
        {
            unitRouter.DeleteRouteVisual(routeVisualId);
        }

        routeVisualId = unitRouter.CreateRouteVisual(assignedRoute);
    }

    if (routeVisualId != -1)
    {
        // We have an active route, so render it.
        unitRouter.Render(projectionMatrix, routeVisualId, isSelected);
    }

    // We do a bunch of matrix math (but nothing to complex) to properly draw armor, bodies, and turrets.
    vec::mat4 unitOrientation = MatrixOps::Translate(position) * rotation.asMatrix();

    const BodyType& bodyType = BodyConfig::Bodies[bodyTypeId];

    // TODO remove, used for click selection. MOVE TO SHADER.
    float scaleFactor = (isSelected ? 1.5f : 1.0f) * bodyType.scale;

    vec::mat4 bodyMatrix = unitOrientation * MatrixOps::Scale(scaleFactor, scaleFactor, scaleFactor);
    modelManager.RenderModel(projectionMatrix, bodyType.bodyModelId, bodyMatrix);

    const ArmorType& armorType = ArmorConfig::Armors[armor.armorTypeId];

    vec::mat4 armorMatrix = MatrixOps::Translate(armorType.translationOffset) * bodyMatrix * armorType.rotationOffset.asMatrix();
    modelManager.RenderModel(projectionMatrix, armorType.armorModelId, armorMatrix);

    for (unsigned int i = 0; i < turrets.size(); i++)
    {
        const TurretType& turretType = TurretConfig::Turrets[turrets[i].turretTypeId];

        vec::mat4 turretDefaultMatrix = MatrixOps::Translate(turretType.translationOffset) * bodyMatrix * turretType.rotationOffset.asMatrix();
        vec::mat4 turretMatrix = MatrixOps::Translate(turrets[i].currentTranslation) * turretDefaultMatrix * turrets[i].currentRotation.asMatrix();
        modelManager.RenderModel(projectionMatrix, turretType.turretModelId, turretMatrix);
    }
}

bool Unit::InRayPath(ModelManager& modelManager, const vec::vec3& rayStart, const vec::vec3& rayVector)
{
    // TODO improve this to do something *drastically* better. Also, this should probably move to the vmath library.

    // Assume the unit is in the ray path if the body, represented as a sphere, is hit by the ray.
    const TextureModel& modelData = modelManager.GetModel(BodyConfig::Bodies[bodyTypeId].bodyModelId);
    float sphereRadius = BodyConfig::Bodies[bodyTypeId].scale * (vec::length(modelData.maxBounds) + vec::length(modelData.minBounds)) / 4.0f;

    // Sphere equation => ((x - pos.x)^2 + (y - pos.y)^2 + (z - pos.z)^2 = sphereRadius^2
    // Ray equation => (rayStart.x + t*rayVector.x, rs.y + t*v.y, rs.z + t*v.z), t >= 0
    // We hit if we can find a value for t that satisfies the above equation.
    // After some work on paper, you end up with just determining if the discriminant of a polynomial is positive
    vec::vec3 rayOffset = rayStart - position;
    float polyC = vec::length(rayOffset * rayOffset) - pow(sphereRadius, 2);

    vec::vec3 polyBComp = rayVector * rayOffset;
    float polyB = polyBComp.x + polyBComp.y + polyBComp.z;

    return (polyB*polyB - polyC) > 0;
}

void Unit::UpdateAssignedRoute(std::vector<vec::vec3> newAssignedRoute)
{
    assignedRoute = newAssignedRoute;
    routeNeedsVisualUpdate = true;
}

void Unit::Move(vec::vec3 pos)
{
    position = pos;
}
