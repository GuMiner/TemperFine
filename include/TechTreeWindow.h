#pragma once
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <vector>
#include "GuiWindow.h"
#include "TechInfo.h"

// Represents a technology tile shown in the tree.
struct TechTile
{
    sfg::Image::Ptr image;
    sfg::Frame::Ptr frame;
};

// Represents the technology tree.
class TechTreeWindow : public GuiWindow
{
    public:
        TechTreeWindow();

    protected:
        virtual bool Setup();

    private:
        std::vector<sfg::Box::Ptr> techLevelBoxes;
        void CreateTechLevelColumnBoxes();

        std::vector<TechTile> techTiles;
        void LoadTechTilesAndFillTechLevels();

        sfg::Box::Ptr masterBox;
        sfg::ScrolledWindow::Ptr scrollableWindow;

        // Extracts out the full name of a tech, given that a tech is reachable from multiple endpoints.
        std::string GetFullTechName(const Tech& tech);

        // Called in the GRAPHICS thread when a tile is clicked on.
        void TechTileHit(unsigned int techId);
};
