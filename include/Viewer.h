#pragma once
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include "KeyBindingConfig.h"
#include "Vec.h"

class Viewer
{
    public:
        vec::vec3 viewPosition;
        vec::quaternion viewOrientation;

        Viewer();
        void InputUpdate();
    protected:
    private:
        sf::Vector2i lastMousePos;
};
