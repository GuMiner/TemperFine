#pragma once
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include "KeyBindingConfig.h"
#include "Vec.h"

class Viewer
{
    public:
        Viewer();
        void InputUpdate();

        const vec::vec3& GetViewPosition() const;
        const vec::quaternion& GetViewOrientation() const;

    private:
        // Where the viewer is currently oriented on-screen.
        vec::vec3 viewPosition;
        vec::quaternion viewOrientation;

        sf::Vector2i lastMousePos;
};
