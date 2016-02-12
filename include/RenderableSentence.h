#pragma once
#include "Vec.h"

struct RenderableSentence
{
    int sentenceId;
    vec::mat4 posRotMatrix;
    vec::vec3 color;
};
