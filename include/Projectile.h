#pragma once
#include "vmath.hpp"

class Projectile
{
    public:
        Projectile();
        void Render(vmath::mat4& projectionMatrix);
        ~Projectile();
    protected:
    private:
        vmath::vec3 position;
        vmath::vec3 currentEulerRotation;
        vmath::quaternion rotation;

        unsigned int modelId;
};
