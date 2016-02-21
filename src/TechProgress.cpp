#include "TechProgress.h"

// Sets an empty tech progress with a user who just has the basic tank.
TechProgress::TechProgress()
{
    minTechLevel = 0;
    maxTechLevel = 0;

    completedTechs.insert(0); // First tech == basic tank.
    UpdateTechs();
}

// Completes a tech.
void TechProgress::CompleteTech(unsigned int tech)
{

}

// True if the tech exists, false otherwise.
bool TechProgress::HasTech(const char* techInternalName)
{
    return true;
}

bool TechProgress::HasTech(unsigned int techId)
{
    return true;
}

// Updates the min/max current tech range and techs available for research.
void TechProgress::UpdateTechs()
{

}