#pragma once
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <vector>
#include "GuiWindow.h"
#include "TechInfo.h"

// Represents a technology tile shown in the tree.
struct TechTile
{
    sfg::Label::Ptr name;
    sfg::Image::Ptr image;

    sfg::Box::Ptr box;
};

// Represents the technology tree.
class TechTreeWindow : public GuiWindow
{
    public:
        TechTreeWindow();

    protected:
        virtual bool Setup();

    private:
        std::vector<TechTile> techTiles;
        std::vector<sfg::Box::Ptr> techLevelBoxes;
        sfg::Box::Ptr masterBox;
        sfg::ScrolledWindow::Ptr scrollableWindow;

        std::string GetFullTechName(const Tech& tech);
};
