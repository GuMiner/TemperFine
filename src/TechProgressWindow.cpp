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
    researchProgress->SetFraction(0.10f);

    researchProgressText = sfg::Label::Create(NoCurrentEta);

    masterBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    masterBox->Pack(techName);
    masterBox->Pack(techImage);
    masterBox->Pack(researchProgress);
    masterBox->Pack(researchProgressText);

    window->Add(masterBox);
    return true;
}

void TechProgressWindow::SetToNoResearch()
{
    techName->SetText(NoCurrentResearch);
    techImage->SetImage(TechConfig::NoCurrentTechImage);
    researchProgress->SetFraction(0.10f);
    researchProgressText->SetText(NoCurrentEta);
}
