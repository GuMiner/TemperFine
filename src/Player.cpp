#include "Player.h"

Player::Player()
{
    name = std::string("Default Player");
}

void Player::RenderUnits(ModelManager& modelManager, vmath::mat4& projectionMatrix)
{
    for (unsigned int i = 0; i < units.size(); i++)
    {
        units[i].Render(modelManager, projectionMatrix);
    }
}

// TODO
int Player::CollisionCheck(vmath::vec3 cameraPos, vmath::vec3 worldRay)
{
    units[0].Move(cameraPos + (worldRay * 6.0f));
    return 0;
}

void Player::AddUnit(const Unit unit)
{
    units.push_back(unit);
}
