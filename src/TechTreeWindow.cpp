#include <sstream>
#include "Logger.h"
#include "TechConfig.h"
#include "TechTreeWindow.h"

TechTreeWindow::TechTreeWindow()
{
}

bool TechTreeWindow::Setup()
{
    window->SetTitle("Technology Tree");
    window->SetPosition(sf::Vector2f(20.0f, 20.0f));

    scrollableWindow = sfg::ScrolledWindow::Create();
    scrollableWindow->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_ALWAYS | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);

    // GetMaxTechLevel
    int maxTechLevel = 0;
    for (unsigned int i = 0; i < TechConfig::Techs.size(); i++)
    {
        if (TechConfig::Techs[i].techLevel > maxTechLevel)
        {
            maxTechLevel = TechConfig::Techs[i].techLevel;
        }
    }

    for (int i = 0; i <= maxTechLevel; i++)
    {
        techLevelBoxes.push_back(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.0f));
    }

    // PackTechLevelBoxesWithTechs
    for (unsigned int i = 0; i < TechConfig::Techs.size(); i++)
    {
        TechTile techTile;
        techTile.image = sfg::Image::Create(TechConfig::Techs[i].techImage);
        techTile.name = sfg::Label::Create(GetFullTechName(TechConfig::Techs[i]));

        techTile.box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.0f);
        techTile.box->SetRequisition(sf::Vector2f(130.0f, 130.0f));
        techTile.box->Pack(techTile.image);
        techTile.box->Pack(techTile.name);

        techTiles.push_back(techTile);
        techLevelBoxes[TechConfig::Techs[i].techLevel]->Pack(techTile.box, false);
    }

    // PackMainBoxWithLevelBoxes
    masterBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.0f);
    for (unsigned int i = 0; i < techLevelBoxes.size(); i++)
    {
        masterBox->Pack(techLevelBoxes[i]);
    }

    // Sets the content and minimum window size, which determines how the scrollbars work.
    scrollableWindow->AddWithViewport(masterBox);
    scrollableWindow->SetRequisition(sf::Vector2f(500.f, 160.f));

    window->Add(scrollableWindow);
    return true;
}

std::string TechTreeWindow::GetFullTechName(const Tech& tech)
{
    std::stringstream nameStream;
    for (unsigned int i = 0; i < tech.names.size(); i++)
    {
        nameStream << tech.names[i];
        if (i != tech.names.size() - 1)
        {
            nameStream << "\n";
        }
    }

    return nameStream.str();
}


