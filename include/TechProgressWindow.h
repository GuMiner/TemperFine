#pragma once
#include <string>
#include "GuiWindow.h"

class TechProgressWindow : public GuiWindow
{
public:
    TechProgressWindow();

    // Sets the research state to the 'No Current Research' state.
    void SetToNoResearch();

protected:
    virtual bool Setup();

private:
    sfg::Label::Ptr techName;
    sfg::Image::Ptr techImage;

    sfg::ProgressBar::Ptr researchProgress;
    sfg::Label::Ptr researchProgressText;

    sfg::Box::Ptr masterBox;

    // 'Statics'
    std::string NoCurrentResearch;
    std::string NoCurrentEta;
};
