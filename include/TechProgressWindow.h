#pragma once
#include <string>
#include "GuiWindow.h"

// Displays the progress in completing the currently-selected tech.
class TechProgressWindow : public GuiWindow
{
public:
    TechProgressWindow();
    
    // Moves the window to screen bottom-left, regardless of current window size.
    void MoveToScreenBottomLeft(sf::Vector2u windowSize);

    // Updates the current research progress, switching techs if necessary.
    void UpdateResearchProgress(unsigned int currentTech, float techProgress);

protected:
    virtual bool Setup();

private:
    // Sets the research state to the 'No Current Research' state.
    void SetToNoResearch();

    // Sets the research state to the given tech.
    void SetToResearch(unsigned int currentTech);

    // Updates the progress of the currently-researching tech.
    void UpdateProgress(float progress);

    // Gets the ETA string for a tech being researched.
    std::string GetEtaString(float progress, int techResearchTime);

    // Last tech known to the program.
    int lastKnownTech;

    sfg::Label::Ptr techName;
    sfg::Image::Ptr techImage;

    sfg::ProgressBar::Ptr researchProgress;
    sfg::Label::Ptr researchProgressText;

    sfg::Box::Ptr masterBox;

    // 'Statics'
    std::string NoCurrentResearch;
    std::string NoCurrentEta;
};
