#include "Logger.h"
#include "TechTreeWindow.h"

TechTreeWindow::TechTreeWindow() : isDisplayed(false)
{
    //ctor
}

bool TechTreeWindow::Initialize(sfg::Desktop* desktop)
{
    this->desktop = desktop;

    // TODO error checking
    if (!techImage.loadFromFile("images/menu/techs.png"))
    {
        Logger::Log("Error loading a technology image specified!");
        return false;
    }

    techsButton = sfg::Button::Create("Techs Button");
    techImageSfg = sfg::Image::Create(techImage);
    techsButton->SetImage(techImageSfg);

    testLabel = sfg::Label::Create("Test Label.");
    testBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.0f);
    testBox->Pack(testLabel);
    testBox->Pack(techsButton);

    window = sfg::Window::Create();
    window->SetTitle("Temper Fine Test Window!");
    window->SetPosition(sf::Vector2f(20.0f, 20.0f));
    window->Add(testBox);

    return true;
}

void TechTreeWindow::Display()
{
    if (!isDisplayed)
    {
        window->Show(true);
        desktop->Add(window);
        isDisplayed = true;
    }
}

void TechTreeWindow::Hide()
{
    if (isDisplayed)
    {
        window->Show(false);
        desktop->Remove(window);
        isDisplayed = false;
    }
}

void TechTreeWindow::ToggleDisplay()
{
    if (isDisplayed)
    {
        Hide();
    }
    else
    {
        Display();
    }
}
