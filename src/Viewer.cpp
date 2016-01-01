#include "PhysicsConfig.h"
#include "Viewer.h"

Viewer::Viewer()
{
    viewPosition = vmath::vec3(3, 4, 2);
	viewOrientation = vmath::quaternion(0, 0, 0, 1);

	lastMousePos = sf::Vector2i(-1, -1);
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

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        if (lastMousePos.x == -1)
        {
            lastMousePos = sf::Mouse::getPosition();
        }
        else
        {
            sf::Vector2i newMousePos = sf::Mouse::getPosition();
            sf::Vector2i deltaPos = lastMousePos - newMousePos;
            lastMousePos = newMousePos;

            // Rotate the camera for x motion and y motion.
            float xAmount = PhysicsConfig::ViewRotateAroundFactor * (float)deltaPos.x;
            float yAmount = PhysicsConfig::ViewRotateUpFactor * (float)deltaPos.y;

            // Rotate around
            viewOrientation = vmath::quaternion::fromAxisAngle(xAmount, vmath::vec3(0, 0, 1)) * viewOrientation;

            // Rotate up
            vmath::vec3 sidewaysVector = vmath::cross(viewOrientation.upVector(), viewOrientation.forwardVector());
            viewOrientation = vmath::quaternion::fromAxisAngle(yAmount, sidewaysVector) * viewOrientation;

            viewOrientation.normalize();
        }
    }
    else
    {
        lastMousePos = sf::Vector2i(-1, -1);
    }
}
