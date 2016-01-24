#include "GuiWindow.h"

GuiWindow::GuiWindow() : isDisplayed(false)
{
}

bool GuiWindow::Initialize(sfg::Desktop* desktop)
{
    this->desktop = desktop;
    window = sfg::Window::Create();

    return Setup();
}

void GuiWindow::Display()
{
    if (!isDisplayed)
    {
        window->Show(true);
        desktop->Add(window);
        isDisplayed = true;
    }
}

void GuiWindow::Hide()
{
    if (isDisplayed)
    {
        window->Show(false);
        desktop->Remove(window);
        isDisplayed = false;
    }
}

void GuiWindow::ToggleDisplay()
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
