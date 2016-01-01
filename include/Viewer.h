#pragma once
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include "KeyBindingConfig.h"
#include "vmath.hpp"

class Viewer
{
    public:
        vmath::vec3 viewPosition;
        vmath::quaternion viewOrientation;

        Viewer();
        void InputUpdate();
    protected:
    private:
};
