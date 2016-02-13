#include "ArmorConfig.h"
#include "BodyConfig.h"
#include "MatrixOps.h"
#include "PhysicsOps.h"
#include "TurretConfig.h"
#include "Unit.h"

Unit::Unit()
{
    routeVisualId = -1;
    routeNeedsVisualUpdate = false;

    testKnownRouteCount = 0;
}

void Unit::TestAddRayPath(vec::vec3 start, vec::vec3 end)
{
    std::vector<vec::vec3> testRoute;
    testRoute.push_back(start);
    testRoute.push_back(end);
    testRoutes.push_back(testRoute);
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

    // TODO test remove
    if (testRoutes.size() != testKnownRouteCount)
    {
        testKnownRouteCount = testRoutes.size();
        testRouteIds.push_back(unitRouter.CreateRouteVisual(testRoutes[testRoutes.size() - 1]));
    }

    // TODO test remove.
    for (int routeId : testRouteIds)
    {
        unitRouter.Render(projectionMatrix, routeId, isSelected);
    }

    // We do a bunch of matrix math (but nothing to complex) to properly draw armor, bodies, and turrets.
    vec::mat4 unitOrientation = MatrixOps::Translate(position) * rotation.asMatrix();

    const BodyType& bodyType = BodyConfig::Bodies[bodyTypeId];

    vec::mat4 bodyMatrix = unitOrientation * MatrixOps::Scale(bodyType.scale, bodyType.scale, bodyType.scale);
    modelManager.RenderModel(projectionMatrix, bodyType.bodyModelId, bodyMatrix, isSelected);

    const ArmorType& armorType = ArmorConfig::Armors[armor.armorTypeId];

    vec::mat4 armorMatrix = MatrixOps::Translate(armorType.translationOffset) * bodyMatrix * armorType.rotationOffset.asMatrix();
    modelManager.RenderModel(projectionMatrix, armorType.armorModelId, armorMatrix, isSelected);

    for (unsigned int i = 0; i < turrets.size(); i++)
    {
        const TurretType& turretType = TurretConfig::Turrets[turrets[i].turretTypeId];

        vec::mat4 turretDefaultMatrix = MatrixOps::Translate(turretType.translationOffset) * bodyMatrix * turretType.rotationOffset.asMatrix();
        vec::mat4 turretMatrix = MatrixOps::Translate(turrets[i].currentTranslation) * turretDefaultMatrix * turrets[i].currentRotation.asMatrix();
        modelManager.RenderModel(projectionMatrix, turretType.turretModelId, turretMatrix, isSelected);
    }
}

bool Unit::InRayPath(ModelManager& modelManager, const vec::vec3& rayStart, const vec::vec3& rayVector)
{
    // TODO improve this to do something *drastically* better.

    // Assume the unit is in the ray path if the body, represented as a sphere, is hit by the ray.
    const TextureModel& modelData = modelManager.GetModel(BodyConfig::Bodies[bodyTypeId].bodyModelId);
    float sphereRadius = 1.0f;//BodyConfig::Bodies[bodyTypeId].scale * (vec::length(modelData.maxBounds) + vec::length(modelData.minBounds)) / 4.0f;

    return PhysicsOps::HitsSphere(rayStart, rayVector, position, sphereRadius);
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
