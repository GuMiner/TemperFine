#pragma once
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

class GuiWindow
{
    public:
        GuiWindow();

        // Sets up the window
        bool Initialize(sfg::Desktop* desktop);

        // Shows the window.
        void Display();

        // Hides the window.
        void Hide();

        // Swaps the window display state (shown/hidden)
        void ToggleDisplay();

    protected:
        // Initialization method for children.
        virtual bool Setup() = 0;

        // The window created for this GUI window.
        sfg::Window::Ptr window;

    private:
        bool isDisplayed;
        sfg::Desktop* desktop;
};