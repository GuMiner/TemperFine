#pragma once
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <vector>
#include "GuiWindow.h"
#include "SharedExclusiveLock.h"
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

        // Tries to get the hit tech tile. Returns true if successful, and the ID of the hit tech tile.
        bool TryGetHitTechTile(unsigned int* techId);

    protected:
        virtual bool Setup();

    private:
        SharedExclusiveLock techTileClickLock;
        bool isTechTileClicked;
        unsigned int hitTechTile;

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
