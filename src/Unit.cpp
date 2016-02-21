#include "ArmorConfig.h"
#include "BodyConfig.h"
#include "Logger.h"
#include "MatrixOps.h"
#include "PhysicsOps.h"
#include "TurretConfig.h"
#include "VecOps.h"
#include "Unit.h"

Unit::Unit()
{
    routeVisualId = -1;
    routeNeedsVisualUpdate = false;
}

// Creates a new unit, with full armor.
// Note that the unrotated unit is pointing in the x direction, with zero rotation.
Unit::Unit(unsigned int armorTypeId, unsigned int bodyTypeId, std::vector<unsigned int> turretTypeIds, const vec::vec3 position, const vec::quaternion rotation) 
    : Unit()
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

// Performs rendering updates that need to be done in the physics thread that don't draw anything.
void Unit::PerformGuiThreadUpdates(RouteVisual& routeVisual)
{
    ReadLock readLock(assignedRouteLock);

    // Update the route visual if the physics system has recomputed it.
    if (routeNeedsVisualUpdate)
    {
        if (routeVisualId != -1)
        {
            routeVisual.DeleteRouteVisual(routeVisualId);
        }

        routeVisualId = routeVisual.CreateRouteVisual(assignedRoute);
        routeNeedsVisualUpdate = false;
    }
}

void Unit::Render(ModelManager& modelManager, RouteVisual& routeVisual, bool isSelected, vec::mat4& projectionMatrix)
{
    ReadLock readLock(unitPhysicsLock);
    
    if (routeVisualId != -1)
    {
        // We have an active route, so render it.
        routeVisual.Render(projectionMatrix, routeVisualId, isSelected);
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

bool Unit::InRayPath(const vec::vec3& rayStart, const vec::vec3& rayVector)
{
    // TODO improve this to do something *drastically* better. Also store unit size data in the unit itself.
    ReadLock readLock(unitPhysicsLock);
    float sphereRadius = 1.0f;
    return PhysicsOps::HitsSphere(rayStart, rayVector, position, sphereRadius);
}

void Unit::UpdateAssignedRoute(std::vector<vec::vec3> newAssignedRoute)
{
    WriteLock writeLock(assignedRouteLock);
    assignedRoute = newAssignedRoute;
    routeNeedsVisualUpdate = true;

    currentSegment = 0;
    currentSegmentPercentage = 0.0f;
}

void Unit::MoveAlongRoute()
{
    ReadLock readLock(assignedRouteLock);
    WriteLock writeLock(unitPhysicsLock);

    // TODO speed needs to be defined here.
    // TODO unit needs to rotate while it moves.
    float travelAmountPerStep = 0.10f;
    if (assignedRoute.size() != 0 && currentSegment != assignedRoute.size() - 1)
    {
        bool finishedTravelling = false;
        while (!finishedTravelling)
        {
            vec::vec3 currentSegmentVector = assignedRoute[currentSegment + 1] - assignedRoute[currentSegment];
            float currentSegmentVectorLength = vec::length(currentSegmentVector);

            float newPercentage = currentSegmentPercentage + travelAmountPerStep / currentSegmentVectorLength;
            if (newPercentage < 1.0f)
            {
                // Length small enough that we don't span segments.
                currentSegmentPercentage = newPercentage;
                position = currentSegmentVector * currentSegmentPercentage + assignedRoute[currentSegment];
                finishedTravelling = true;
            }
            else
            {
                // We span a segment, so iterate to that segment, and exit if we're at the end of the route.
                travelAmountPerStep -= (1.0f - currentSegmentPercentage) * currentSegmentVectorLength;
                currentSegmentPercentage = 0.0f;
                currentSegment++;
                if (currentSegment == assignedRoute.size() - 1)
                {
                    position = assignedRoute[assignedRoute.size() - 1];
                    finishedTravelling = true;
                }
            }
        }
    }
}

void Unit::Move(vec::vec3 pos)
{
    WriteLock writeLock(unitPhysicsLock);
    position = pos;
}
