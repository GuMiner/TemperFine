#include "Player.h"

Player::Player()
{

}

Player::Player(const std::string& name, int id)
{
    this->name = name;
    this->id = id;
}

void Player::AddUnit(const Unit& unit)
{
    WriteLock writeLock(playerUnitVectorMutex);
    units.push_back(unit);
}

void Player::PerformUnitGuiUpdates(RouteVisual& routeVisuals)
{
    ReadLock readLock(playerUnitVectorMutex);
    for (unsigned int i = 0; i < units.size(); i++)
    {
        units[i].PerformGuiThreadUpdates(routeVisuals);
    }
}

void Player::RenderUnits(ModelManager& modelManager, RouteVisual& routeVisual, vec::mat4& projectionMatrix)
{
    ReadLock readLock(playerUnitVectorMutex);
    ReadLock readLock2(unitSelectionMutex);
    for (unsigned int i = 0; i < units.size(); i++)
    {
        units[i].Render(modelManager, routeVisual, selectedUnits.find(i) != selectedUnits.end(), projectionMatrix);
    }
}

int Player::CollisionCheck(vec::vec3 cameraPos, vec::vec3 worldRay)
{
    ReadLock readLock(playerUnitVectorMutex);
    for (unsigned int i = 0; i < units.size(); i++)
    {
        if (units[i].InRayPath(cameraPos, worldRay))
        {
            return i;
        }
    }

    return -1;
}

void Player::UpdateUnitRoute(int unitId, const std::vector<vec::vec3>& route)
{
    ReadLock readLock(playerUnitVectorMutex);
    units[unitId].UpdateAssignedRoute(route);
}

void Player::MoveUnits()
{
    ReadLock readLock(playerUnitVectorMutex);
    for (unsigned int i = 0; i < units.size(); i++)
    {
        units[i].MoveAlongRoute();
    }
}

void Player::ToggleUnitSelection(int unitId)
{
    WriteLock writeLock(unitSelectionMutex);
    std::set<int>::const_iterator searchResult = selectedUnits.find(unitId);
    if (searchResult == selectedUnits.end())
    {
        // Not found, select
        selectedUnits.insert(unitId);
    }
    else
    {
        // Found, deselect
        selectedUnits.erase(searchResult);
    }
}

// Returns the players selected units. 
const std::set<int>& Player::GetSelectedUnits() const
{
    return selectedUnits;
}
