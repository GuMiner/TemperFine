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

bool GuiWindow::WithinVisibleBounds(int xp, int yp)
{
    if (isDisplayed)
    {
        sf::FloatRect rect = window->GetAllocation();
        return (xp > rect.left && yp > rect.top && xp < rect.left + rect.width && yp < rect.top + rect.height);
    }

    // Window not visible, nothing to hit.
    return false;
}
