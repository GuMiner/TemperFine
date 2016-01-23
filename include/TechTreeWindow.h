#pragma once
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

class TechTreeWindow
{
    public:
        TechTreeWindow();
        bool Initialize(sfg::Desktop* desktop);

        void Display();
        void Hide();
        void ToggleDisplay();

    private:
        bool isDisplayed;
        sfg::Desktop* desktop;

        sf::Image techImage;

        sfg::Label::Ptr testLabel;
        sfg::Button::Ptr techsButton;
        sfg::Image::Ptr techImageSfg;

        sfg::Box::Ptr testBox;
        sfg::Window::Ptr window;
};
