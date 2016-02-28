#include <sstream>
#include "TechConfig.h"
#include "TechProgressWindow.h"

TechProgressWindow::TechProgressWindow()
{
    NoCurrentResearch = std::string("No current research");
    NoCurrentEta = std::string("No remaining research time.");
}

bool TechProgressWindow::Setup()
{
    window->SetTitle("Current Research");
    window->SetPosition(sf::Vector2f(10.0f, 20.0f));

    techName = sfg::Label::Create(NoCurrentResearch);
    techImage = sfg::Image::Create(TechConfig::NoCurrentTechImage);

    researchProgress = sfg::ProgressBar::Create(sfg::ProgressBar::Orientation::HORIZONTAL);
    
    researchProgressText = sfg::Label::Create(NoCurrentEta);

    masterBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    masterBox->Pack(techName);
    masterBox->Pack(techImage);
    masterBox->Pack(researchProgress);
    masterBox->Pack(researchProgressText);

    window->Add(masterBox);
    window->SetStyle(sfg::Window::BACKGROUND);

    SetToNoResearch();
    return true;
}

void TechProgressWindow::MoveToScreenBottomLeft(sf::Vector2u windowSize)
{
    const sf::FloatRect allocation = window->GetAllocation();
    window->SetPosition(sf::Vector2f(0, windowSize.y - allocation.height));
}

void TechProgressWindow::UpdateResearchProgress(unsigned int currentTech, float techProgress)
{
    if (currentTech != lastKnownTech)
    {
        if (currentTech == -1)
        {
            SetToNoResearch();
        }
        else
        {
            SetToResearch(currentTech);
        }
    }
    else if (lastKnownTech != -1)
    {
        UpdateProgress(techProgress);
    }
}

void TechProgressWindow::SetToNoResearch()
{
    lastKnownTech = -1;
    techName->SetText(NoCurrentResearch);
    techImage->SetImage(TechConfig::NoCurrentTechImage);
    researchProgress->SetFraction(0.10f);
    researchProgressText->SetText(NoCurrentEta);
}

void TechProgressWindow::SetToResearch(unsigned int currentTech)
{
    lastKnownTech = currentTech;
    techName->SetText(TechConfig::Techs[currentTech].names[0]);
    techImage->SetImage(TechConfig::Techs[currentTech].techImage);
    researchProgress->SetFraction(0.0f);
    researchProgressText->SetText(NoCurrentEta);
}

void TechProgressWindow::UpdateProgress(float progress)
{
    researchProgress->SetFraction(progress);
    researchProgressText->SetText(GetEtaString(progress, TechConfig::Techs[lastKnownTech].researchTimeSeconds));
}

std::string TechProgressWindow::GetEtaString(float progress, int techResearchTime)
{
    std::stringstream etaString;
    etaString.precision(2);
    etaString << std::fixed;
    etaString << ((1.0f - progress) * (float)techResearchTime) << " s";
    return etaString.str();
}