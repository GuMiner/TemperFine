#include "BuildingsWindow.h"

BuildingsWindow::BuildingsWindow()
{
// TODO
}

bool BuildingsWindow::Setup()
{
    window->SetTitle("Buildings");

    masterBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    // TODO add buildings to the master box.

    window->Add(masterBox);
    return true;
}