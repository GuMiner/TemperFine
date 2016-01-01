#include "PhysicsConfig.h"
#include "Viewer.h"

Viewer::Viewer()
{
    viewPosition = vmath::vec3(3, 4, 2);
	viewOrientation = vmath::quaternion(0, 0, 0, 1);
}

void Viewer::InputUpdate()
{
    if (sf::Keyboard::isKeyPressed(KeyBindingConfig::MoveLeft))
    {
        viewPosition[0] -= PhysicsConfig::ViewSidewaysSpeed;
    }

    if (sf::Keyboard::isKeyPressed(KeyBindingConfig::MoveRight))
    {
        viewPosition[0] += PhysicsConfig::ViewSidewaysSpeed;
    }

    if (sf::Keyboard::isKeyPressed(KeyBindingConfig::MoveUp))
    {
        viewPosition[1] -= PhysicsConfig::ViewForwardsSpeed;
    }

    if (sf::Keyboard::isKeyPressed(KeyBindingConfig::MoveDown))
    {
        viewPosition[1] += PhysicsConfig::ViewForwardsSpeed;
    }
}
