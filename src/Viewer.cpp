#include "PhysicsConfig.h"
#include "VecOps.h"
#include "Viewer.h"

Viewer::Viewer()
{
    viewPosition = vmath::vec3(0, 0, 10);
	viewOrientation = vmath::quaternion(0, 0, 0, 1);

	lastMousePos = sf::Vector2i(-1, -1);
}

void Viewer::InputUpdate()
{
    vmath::vec3 forwardsVector = viewOrientation.forwardVector();
    vmath::vec3 sidewaysVector = VecOps::Cross(viewOrientation.upVector(), forwardsVector);
    if (sf::Keyboard::isKeyPressed(KeyBindingConfig::MoveLeft))
    {
        viewPosition -= sidewaysVector * PhysicsConfig::ViewSidewaysSpeed;
    }

    if (sf::Keyboard::isKeyPressed(KeyBindingConfig::MoveRight))
    {
        viewPosition += sidewaysVector * PhysicsConfig::ViewSidewaysSpeed;
    }

    forwardsVector[2] = 0; // Moving forwards doesn't move you down in the Z-direction.
    if (forwardsVector[0] < 0.01f && forwardsVector[0] > -0.01f)
    {
        forwardsVector[0] = 0.01f; // Tiny nudge to avoid div/zero issues.
    }

    forwardsVector = vmath::normalize(forwardsVector);

    if (sf::Keyboard::isKeyPressed(KeyBindingConfig::MoveUp))
    {
        viewPosition += forwardsVector * PhysicsConfig::ViewForwardsSpeed;
    }

    if (sf::Keyboard::isKeyPressed(KeyBindingConfig::MoveDown))
    {
        viewPosition -= forwardsVector * PhysicsConfig::ViewForwardsSpeed;
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
            vmath::vec3 sidewaysVector = VecOps::Cross(viewOrientation.upVector(), viewOrientation.forwardVector());
            viewOrientation = vmath::quaternion::fromAxisAngle(yAmount, sidewaysVector) * viewOrientation;

            viewOrientation.normalize();
        }
    }
    else
    {
        lastMousePos = sf::Vector2i(-1, -1);
    }
}
