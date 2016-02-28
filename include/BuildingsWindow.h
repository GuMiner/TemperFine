#pragma once
#include "GuiWindow.h"

// Displays the list of buildings currently-available for building. 
// Used for click (building)-click(building spot) building strategy.
class BuildingsWindow : public GuiWindow
{
public:
    BuildingsWindow();

protected:
    virtual bool Setup();

private:
    // TODO should hold all the possible buildings and only list those available given the current research.
    // TODO we need a buildings config file to hold the buildings, their required techs, and images.
    sfg::Box::Ptr masterBox;
};