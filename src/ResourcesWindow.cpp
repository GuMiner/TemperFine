#include <sstream>
#include "ResourcesWindow.h"

ResourcesWindow::ResourcesWindow()
{ }

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
    return true;
}

// Updates the stored resources.
void ResourcesWindow::UpdateStoredResources(float storedTime, float storedFuel)
{
    timeLabel->SetText(GetTimeString(storedTime));
    fuelLabel->SetText(GetFuelString(storedFuel));

    // TODO also update the min, max, and progress bars.
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