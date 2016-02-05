#include "Player.h"

Player::Player()
{
    name = std::string("Default Player");
}

void Player::RenderUnits(ModelManager& modelManager, vmath::mat4& projectionMatrix)
{
    for (unsigned int i = 0; i < units.size(); i++)
    {
        units[i].Render(modelManager, selectedUnits.find(i) != selectedUnits.end(), projectionMatrix);
    }
}

// TODO
int Player::CollisionCheck(ModelManager& modelManager, vmath::vec3 cameraPos, vmath::vec3 worldRay)
{
    for (unsigned int i = 0; i < units.size(); i++)
    {
        if (units[i].InRayPath(modelManager, cameraPos, worldRay))
        {
            return i;
        }
    }

    return -1;
}

void Player::ToggleUnitSelection(int unitId)
{
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

void Player::AddUnit(const Unit unit)
{
    units.push_back(unit);
}
