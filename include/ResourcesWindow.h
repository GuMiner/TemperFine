#pragma once
#include "GuiWindow.h"

// Displays the current available resource for the player.
class ResourcesWindow : public GuiWindow
{
public:
    ResourcesWindow();

    // Moves the resources window to the bottom-middle of the screen, regardless of screen size.
    void MoveToScreenBottom(sf::Vector2u windowSize);

    // Updates the stored resources.
    void UpdateStoredResources(float storedTime, float storedFuel);

protected:
    virtual bool Setup();

private:
    // Gets user-friendly strings for fuel amounts and time amounts.
    std::string GetFuelString(float fuelAmount);
    std::string GetTimeString(float timeAmount);

    // Updates the max time and fuel amounts for proper scrollbars.
    void UpdateMaxTimeFuelAmounts(float timeAmount, float fuelAmount);

    int minimumMaxAmount;

    int lastMaxTime;
    int lastMaxFuel;

    int maxTime;
    int maxFuel;

    sfg::Label::Ptr fuelLabel;
    sfg::Label::Ptr timeLabel;

    sfg::Label::Ptr minTimeAmount;
    sfg::Label::Ptr minFuelAmount;

    sfg::ProgressBar::Ptr storedTimeBar;
    sfg::ProgressBar::Ptr storedFuelBar;

    sfg::Label::Ptr maxTimeAmount;
    sfg::Label::Ptr maxFuelAmount;

    sfg::Box::Ptr timeHorizontalBox;
    sfg::Box::Ptr fuelHorizontalBox;
    sfg::Box::Ptr masterBox;
};