#include "TechConfig.h"
#include "Player.h"

Player::Player()
{
    currentlyResearchingTech = -1;
    elapsedResearchTimeSeconds = 0.0f;
    storedFuelAmount = 0.0f;
}

Player::Player(const std::string& name, int id) : Player()
{
    this->name = name;
    this->id = id;
}

void Player::AddUnit(const Unit& unit)
{
    WriteLock writeLock(playerUnitVectorMutex);
    units.push_back(unit);
}

void Player::PerformUnitGuiUpdates(RouteVisual& routeVisuals)
{
    ReadLock readLock(playerUnitVectorMutex);
    for (unsigned int i = 0; i < units.size(); i++)
    {
        units[i].PerformGuiThreadUpdates(routeVisuals);
    }
}

void Player::RenderUnits(ModelManager& modelManager, RouteVisual& routeVisual, vec::mat4& projectionMatrix)
{
    ReadLock readLock(playerUnitVectorMutex);
    ReadLock readLock2(unitSelectionMutex);
    for (unsigned int i = 0; i < units.size(); i++)
    {
        units[i].Render(modelManager, routeVisual, selectedUnits.find(i) != selectedUnits.end(), projectionMatrix);
    }
}

int Player::CollisionCheck(vec::vec3 cameraPos, vec::vec3 worldRay)
{
    ReadLock readLock(playerUnitVectorMutex);
    for (unsigned int i = 0; i < units.size(); i++)
    {
        if (units[i].InRayPath(cameraPos, worldRay))
        {
            return i;
        }
    }

    return -1;
}

void Player::UpdateUnitRoute(int unitId, const std::vector<vec::vec3>& route)
{
    ReadLock readLock(playerUnitVectorMutex);
    units[unitId].UpdateAssignedRoute(route);
}

void Player::MoveUnits()
{
    ReadLock readLock(playerUnitVectorMutex);
    for (unsigned int i = 0; i < units.size(); i++)
    {
        units[i].MoveAlongRoute();
    }
}

bool Player::SwitchResearch(unsigned int techId)
{
    WriteLock writeLock(techProgressMutex);

    if (techProgress.CanStartTech(techId))
    {
        currentlyResearchingTech = techId;
        return true;
    }

    return false;
}

void Player::UpdateResearchProgress(float lastElapsedTime)
{
    WriteLock writeLock(techProgressMutex);

    // TODO research multiplier by the number of research buildings the player has.
    elapsedResearchTimeSeconds += lastElapsedTime;

    // Note that we always add the research time, so your research buildings will keep accruing research points even if not
    //  actively researching anything. This reduces the time pressure in selecting a new research topic.
    if (currentlyResearchingTech != -1)
    {
        int techResearchTimeSec = TechConfig::Techs[currentlyResearchingTech].researchTimeSeconds;
        if (techResearchTimeSec <= (int)elapsedResearchTimeSeconds)
        {
            techProgress.CompleteTech(currentlyResearchingTech);
            currentlyResearchingTech = -1;
            elapsedResearchTimeSeconds -= (float)techResearchTimeSec;
        }
    }
}

int Player::GetResearchProgress(float* researchFraction)
{
    ReadLock readLock(techProgressMutex);
    if (currentlyResearchingTech != -1)
    {
        *researchFraction = elapsedResearchTimeSeconds / (float)TechConfig::Techs[currentlyResearchingTech].researchTimeSeconds;
        if (*researchFraction > 1.0f)
        {
            *researchFraction = 1.0f;
        }
    }

    return currentlyResearchingTech;
}

void Player::GetStoredResources(float* storedTime, float* storedFuel)
{
    ReadLock readLock(techProgressMutex);
    ReadLock readLock2(fuelMutex);

    *storedTime = elapsedResearchTimeSeconds;
    *storedFuel = storedFuelAmount;
}

void Player::ToggleUnitSelection(int unitId)
{
    WriteLock writeLock(unitSelectionMutex);
    std::set<int>::const_iterator searchResult = selectedUnits.find(unitId);
    if (searchResult == selectedUnits.end())
    {
        // Not found, select
        selectedUnits.insert(unitId);
    }
    else
    {
        // Found, deselect
        selectedUnits.erase(searchResult);
    }
}

// Returns the players selected units. 
const std::set<int>& Player::GetSelectedUnits() const
{
    return selectedUnits;
}
