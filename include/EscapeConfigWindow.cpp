#include "EscapeConfigWindow.h"

EscapeConfigWindow::EscapeConfigWindow()
{
    shouldQuit = false;
}

bool EscapeConfigWindow::Setup()
{
    window->SetTitle("TemperFine Paused");

    exitButton = sfg::Button::Create("Quit");
    exitButton->GetSignal(sfg::Button::OnLeftClick).Connect(
        std::bind(&EscapeConfigWindow::ExitButtonHit, this));

    masterBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    masterBox->Pack(exitButton); 
    window->Add(masterBox);
    return true;
}

bool EscapeConfigWindow::ShouldQuit()
{
    return shouldQuit;
}

void EscapeConfigWindow::ExitButtonHit()
{
    shouldQuit = true;
}