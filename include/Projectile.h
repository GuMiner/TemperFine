#pragma once
#include "Vec.h"

class Projectile
{
    public:
        Projectile();
        void Render(vec::mat4& projectionMatrix);
        ~Projectile();
    protected:
    private:
        vec::vec3 position;
        vec::vec3 currentEulerRotation;
        vec::quaternion rotation;

        unsigned int modelId;
};
