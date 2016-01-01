#include "Viewer.h"

Viewer::Viewer()
{
    viewPosition = vmath::vec3(65, 0, 0);
	viewOrientation = vmath::quaternion(0, 0, 0, 1);
}

void Viewer::InputUpdate()
{
    if (sf::Keyboard::isKeyPressed(KeyBindingConfig::MoveLeft))
    {
        viewPosition[0] -= 5.0f;
    }

    if (sf::Keyboard::isKeyPressed(KeyBindingConfig::MoveRight))
    {
        viewPosition[0] += 5.0f;
    }

    if (sf::Keyboard::isKeyPressed(KeyBindingConfig::MoveUp))
    {
        viewPosition[1] -= 5.0f;
    }

    if (sf::Keyboard::isKeyPressed(KeyBindingConfig::MoveDown))
    {
        viewPosition[1] += 5.0f;
    }
}
