#pragma once
#include "FontManager.h"
#include "Vertex.h"
#include "vmath.hpp"

class Statistics
{
    // Holds a reference to the font manager so we can update text sentences
    FontManager* fontManager;

    public:
        Statistics();
        bool Initialize(FontManager* fontManager);
        void UpdateStats(vmath::vec3& position);
        void RenderStats(vmath::mat4& perspectiveMatrix);
    protected:
    private:
        int textPixelHeight;
        vmath::mat4 textScale;

        int xPositionSentence;
        vmath::mat4 xPositionMatrix;
        vmath::vec3 xPositionColor;

        int yPositionSentence;
        vmath::mat4 yPositionMatrix;
        vmath::vec3 yPositionColor;

        int zPositionSentence;
        vmath::mat4 zPositionMatrix;
        vmath::vec3 zPositionColor;
};
