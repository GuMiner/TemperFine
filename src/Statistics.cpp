#include <sstream>
#include <string>
#include "Statistics.h"

Statistics::Statistics()
{
    textPixelHeight = 20;

    textScale = vmath::scale(0.02f, 0.02f, 0.02f);

    // TODO hud depth should not be -1.0f?
    xPositionMatrix = vmath::translate(-0.821f, -0.321f, -1.0f) * textScale;
    yPositionMatrix = vmath::translate(-0.659f, -0.321f, -1.0f) * textScale;
    zPositionMatrix = vmath::translate(-0.508f, -0.321f, -1.0f) * textScale;

    xPositionColor = vmath::vec3(1.0f, 0.0f, 0.0f);
    yPositionColor = vmath::vec3(0.0f, 1.0f, 0.0f);
    zPositionColor = vmath::vec3(0.0f, 0.0f, 1.0f);
}

bool Statistics::Initialize(FontManager* fontManager)
{
    this->fontManager = fontManager;

    // Create the sentence objects to perform font manipulations on.
    xPositionSentence = fontManager->CreateNewSentence();
    yPositionSentence = fontManager->CreateNewSentence();
    zPositionSentence = fontManager->CreateNewSentence();

    return true;
}

void Statistics::UpdateStats(vmath::vec3& position)
{
    std::stringstream textStream;
    textStream.precision(2);
    textStream << std::fixed;

    textStream << "X: " << position[0];
    fontManager->UpdateSentence(xPositionSentence, textStream.str().c_str(), textPixelHeight, xPositionColor);

    textStream.str("");
    textStream << "Y: " << position[1];
    fontManager->UpdateSentence(yPositionSentence, textStream.str(), textPixelHeight, yPositionColor);

    textStream.str("");
    textStream << "Z: " << position[2];
    fontManager->UpdateSentence(zPositionSentence, textStream.str(), textPixelHeight, zPositionColor);
}

void Statistics::RenderStats(vmath::mat4& perspectiveMatrix)
{
    fontManager->RenderSentence(xPositionSentence, perspectiveMatrix, xPositionMatrix);
    fontManager->RenderSentence(yPositionSentence, perspectiveMatrix, yPositionMatrix);
    fontManager->RenderSentence(zPositionSentence, perspectiveMatrix, zPositionMatrix);
}
