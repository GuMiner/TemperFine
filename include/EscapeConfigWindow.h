#pragma once
#include "GuiWindow.h"

// Window that appears when Escape is paused (which also pauses the game), for config or game exiting.
class EscapeConfigWindow : public GuiWindow
{
public:
    EscapeConfigWindow();

    // True if the exit button is hit, false otherwise.
    bool ShouldQuit();

protected:
    virtual bool Setup();

private:
    bool shouldQuit;
    void ExitButtonHit();

    // TODO implement the details for this.
    sfg::Button::Ptr exitButton;
    sfg::Box::Ptr masterBox;
};