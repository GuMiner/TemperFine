#pragma once
#include <string>
#include "FontManager.h"
#include "RenderableSentence.h"
#include "Vertex.h"
#include "vmath.hpp"

// Draws statistics information in a more fancier (and older) manner than using the GUI library.
//  This information is drawn with the FontManager global font.
class Statistics
{
    // Holds a reference to the font manager so we can update text sentences
    FontManager* fontManager;

    public:
        Statistics();
        bool Initialize(FontManager* fontManager);

        void UpdateRunTime(float currentTime);
        void UpdateViewPos(vmath::vec3& position);
        void UpdateTechLevelRange(int minLevel, int maxLevel);
        void UpdatePlayerDetails(std::string& playerName);

        void RenderStats(vmath::mat4& perspectiveMatrix);

    private:
        int textPixelHeight;
        vmath::mat4 textScale;

        // Overall Details
        RenderableSentence playerCount;
        RenderableSentence runTime;

        // Current player details.
        RenderableSentence playerName;
        RenderableSentence playerMinTechLevel;
        RenderableSentence playerMaxTechLevel;

        // (Viewer details (for current player))
        RenderableSentence xPosition;
        RenderableSentence yPosition;
        RenderableSentence zPosition;
};
