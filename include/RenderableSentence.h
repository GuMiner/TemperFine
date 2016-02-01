#pragma once
#include "vmath.hpp"

struct RenderableSentence
{
    int sentenceId;
    vmath::mat4 posRotMatrix;
    vmath::vec3 color;
};
