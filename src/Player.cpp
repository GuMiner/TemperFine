#include "Player.h"

Player::Player()
{
    name = std::string("Default Player");
}

void Player::RenderUnits(ModelManager& modelManager, vmath::mat4& projectionMatrix)
{
    for (int i = 0; i < units.size(); i++)
    {
        units[i].Render(modelManager, projectionMatrix);
    }
}

void Player::AddUnit(const Unit unit)
{
    units.push_back(unit);
}
