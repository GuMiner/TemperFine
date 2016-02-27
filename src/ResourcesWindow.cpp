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
    window->SetPosition(sf::Vector2f(20.0f, 10.0f));

    timeLabel = sfg::Label::Create("");
    fuelLabel = sfg::Label::Create("");

    minTimeAmount = sfg::Label::Create("0");
    minFuelAmount = sfg::Label::Create("0");

    storedTimeBar = sfg::ProgressBar::Create(sfg::ProgressBar::Orientation::HORIZONTAL);

    storedFuelBar = sfg::ProgressBar::Create(sfg::ProgressBar::Orientation::HORIZONTAL);

    maxTimeAmount = sfg::Label::Create("0");
    maxFuelAmount = sfg::Label::Create("0");

    timeHorizontalBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    timeHorizontalBox->SetRequisition(sf::Vector2f(200.0f, 20.0f));
    fuelHorizontalBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    fuelHorizontalBox->SetRequisition(sf::Vector2f(200.0f, 20.0f));

    timeHorizontalBox->Pack(minTimeAmount);
    fuelHorizontalBox->Pack(minFuelAmount);

    timeHorizontalBox->Pack(storedTimeBar);
    fuelHorizontalBox->Pack(storedFuelBar);

    timeHorizontalBox->Pack(maxTimeAmount);
    fuelHorizontalBox->Pack(maxFuelAmount);

    masterBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    masterBox->Pack(timeLabel);
    masterBox->Pack(timeHorizontalBox);
    masterBox->Pack(fuelLabel);
    masterBox->Pack(fuelHorizontalBox);

    window->Add(masterBox);
    window->SetStyle(sfg::Window::BACKGROUND);
    return true;
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
    // Scale up as needed.
    while (timeAmount > maxTime)
    {
        lastMaxTime = maxTime;
        maxTime *= 10;
    }

    while (fuelAmount > maxFuel)
    {
        lastMaxFuel = maxFuel;
        maxFuel *= 10;
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

        maxTime /= 10;
        lastMaxTime /= 10;
    }

    while (fuelAmount < lastMaxFuel)
    {
        if (maxFuel == minimumMaxAmount)
        {
            // Can't go lower!
            lastMaxFuel = maxFuel;
            break;
        }

        maxFuel /= 10;
        lastMaxFuel /= 10;
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
    fuelString << "Fuel: " << fuelAmount;
    return fuelString.str();
}

std::string ResourcesWindow::GetTimeString(float timeAmount)
{
    std::stringstream timeString;
    timeString.precision(1);
    timeString << std::fixed;
    timeString << "Research: " << timeAmount;
    return timeString.str();
}