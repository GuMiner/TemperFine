#include <sstream>
#include "ResourcesWindow.h"

ResourcesWindow::ResourcesWindow()
{
    // Constant
    minimumMaxAmount = 1;

    // Varying window for the scrollbar min/max amounts.
    lastMaxTime = 1;
    lastMaxFuel = 1;
    maxTime = 1;
    maxFuel = 1;
}

bool ResourcesWindow::Setup()
{
    window->SetTitle("Resources");

    timeLabel = sfg::Label::Create("");
    fuelLabel = sfg::Label::Create("");

    minTimeAmount = sfg::Label::Create("0");
    minFuelAmount = sfg::Label::Create("0");

    storedTimeBar = sfg::ProgressBar::Create(sfg::ProgressBar::Orientation::HORIZONTAL);
    storedTimeBar->SetRequisition(sf::Vector2f(200, 10));
    storedFuelBar = sfg::ProgressBar::Create(sfg::ProgressBar::Orientation::HORIZONTAL);
    storedFuelBar->SetRequisition(sf::Vector2f(200, 10));

    maxTimeAmount = sfg::Label::Create("0");
    maxFuelAmount = sfg::Label::Create("0");

    timeHorizontalBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    fuelHorizontalBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);

    timeHorizontalBox->Pack(timeLabel);
    fuelHorizontalBox->Pack(fuelLabel);
    timeHorizontalBox->Pack(minTimeAmount);
    fuelHorizontalBox->Pack(minFuelAmount);

    timeHorizontalBox->Pack(storedTimeBar);
    fuelHorizontalBox->Pack(storedFuelBar);

    timeHorizontalBox->Pack(maxTimeAmount);
    fuelHorizontalBox->Pack(maxFuelAmount);

    masterBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    masterBox->Pack(timeHorizontalBox);
    masterBox->Pack(fuelHorizontalBox);

    window->Add(masterBox);
    window->SetStyle(sfg::Window::BACKGROUND);
    window->SetRequisition(sf::Vector2f(400, 30));
    return true;
}

// Moves the resources window to the bottom-middle of the screen, regardless of screen size.
void ResourcesWindow::MoveToScreenBottom(sf::Vector2u windowSize)
{
    const sf::FloatRect allocation = window->GetAllocation();
    window->SetPosition(sf::Vector2f(windowSize.x / 2 - allocation.width / 2, windowSize.y - allocation.height));
}

// Updates the stored resources.
void ResourcesWindow::UpdateStoredResources(float storedTime, float storedFuel)
{
    timeLabel->SetText(GetTimeString(storedTime));
    fuelLabel->SetText(GetFuelString(storedFuel));

    // Updates the max and progress bars
    UpdateMaxTimeFuelAmounts(storedTime, storedFuel);
    storedTimeBar->SetFraction(storedTime / (float)maxTime);
    storedFuelBar->SetFraction(storedFuel / (float)maxFuel);
}

void ResourcesWindow::UpdateMaxTimeFuelAmounts(float timeAmount, float fuelAmount)
{
    const int scaleAmount = 2;

    // Scale up as needed.
    while (timeAmount > maxTime)
    {
        lastMaxTime = maxTime;
        maxTime *= scaleAmount;
    }

    while (fuelAmount > maxFuel)
    {
        lastMaxFuel = maxFuel;
        maxFuel *= scaleAmount;
    }

    // Scale down as needed
    while (timeAmount < lastMaxTime)
    {
        if (maxTime == minimumMaxAmount)
        {
            // Can't go lower!
            lastMaxTime = maxTime;
            break;
        }

        maxTime /= scaleAmount;
        lastMaxTime /= scaleAmount;
    }

    while (fuelAmount < lastMaxFuel)
    {
        if (maxFuel == minimumMaxAmount)
        {
            // Can't go lower!
            lastMaxFuel = maxFuel;
            break;
        }

        maxFuel /= scaleAmount;
        lastMaxFuel /= scaleAmount;
    }

    std::stringstream maxTimeString;
    maxTimeString << maxTime << " s";
    maxTimeAmount->SetText(maxTimeString.str());

    std::stringstream maxFuelString;
    maxFuelString << maxFuel << " kg";
    maxFuelAmount->SetText(maxFuelString.str());
}

std::string ResourcesWindow::GetFuelString(float fuelAmount)
{
    std::stringstream fuelString;
    fuelString.precision(1);
    fuelString << std::fixed;
    fuelString << "Fuel: " << fuelAmount << " kg";
    return fuelString.str();
}

std::string ResourcesWindow::GetTimeString(float timeAmount)
{
    std::stringstream timeString;
    timeString.precision(1);
    timeString << std::fixed;
    timeString << "Research: " << timeAmount << " s";
    return timeString.str();
}