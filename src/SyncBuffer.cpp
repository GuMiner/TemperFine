#include "MatrixOps.h"
#include "SyncBuffer.h"

SyncBuffer::SyncBuffer()
{
    roundMapUpdatedVisuals = false;
    roundMapUpdatedPhysics = false;
    newSelectedVoxel = false;
}

// Adds the player to the players in the round.
void SyncBuffer::AddPlayer(const std::string& playerName)
{
    WriteLock writeLock(playerVectorMutex);
    gameRound.players.push_back(Player(playerName, (int)gameRound.players.size()));
}

// Locks a player for direct physics-thread use.
Player& SyncBuffer::LockPlayer(unsigned int playerId)
{
    playerVectorMutex.ReadLock();
    return gameRound.players[playerId];
}

// Unlocks a player for direct physics-thread use.
void SyncBuffer::UnlockPlayer(unsigned int playerId)
{
    // playerId is unused
    playerVectorMutex.ReadUnlock();
}

void SyncBuffer::RenderPlayers(ModelManager& modelManager, RouteVisual& routeVisuals, vec::mat4& projectionMatrix)
{
    ReadLock readLock(playerVectorMutex);
    for (unsigned int i = 0; i < gameRound.players.size(); i++)
    {
        gameRound.players[i].PerformUnitGuiUpdates(routeVisuals);
        gameRound.players[i].RenderUnits(modelManager, routeVisuals, projectionMatrix);
    }
}

void SyncBuffer::UpdatePlayers()
{
    ReadLock readLock(playerVectorMutex);
    for (unsigned int i = 0; i < gameRound.players.size(); i++)
    {
        gameRound.players[i].MoveUnits();
    }
}

// Sets the round map.
void SyncBuffer::SetRoundMap(const MapInfo& testMap)
{
    WriteLock writeLock(mapUpdateMutex);
    gameRound.map = testMap;
    roundMapUpdatedVisuals = true;
    roundMapUpdatedPhysics = true;
}

bool SyncBuffer::UpdateRoundMapPhysics(MapSections& mapSections)
{
    if (roundMapUpdatedPhysics)
    {
        // Note that because we don't 'unset' the boolean and there's only one reader,
        //  we don't need to check the if-block again.
        ReadLock readLock(mapUpdateMutex);
        mapSections.RecomputeMapSections(gameRound.map);
        roundMapUpdatedPhysics = false;

        return true;
    }

    return false;
}

// Wrapper to MapSections HitByRay method, locking and providing the map.
bool SyncBuffer::HitByRay(MapSections& mapSections, const vec::vec3& rayStart, const vec::vec3& rayVector, vec::vec3i* voxelId)
{
    ReadLock readLock(mapUpdateMutex);
    return mapSections.HitByRay(&gameRound.map, rayStart, rayVector, voxelId);
}

void SyncBuffer::RefineRoute(UnitRouter& unitRouter, const voxelSubsectionsMap& voxelSubsections, const vec::vec3i start, const vec::vec3i destination,
    const std::vector<vec::vec3i>& givenPath, std::vector<vec::vec3i>& refinedPath, std::vector<vec::vec3>& visualPath)
{
    ReadLock readLock(mapUpdateMutex);
    return unitRouter.RefineRoute(&gameRound.map, voxelSubsections, start, destination, givenPath, refinedPath, visualPath);
}

// Updates the round map display. Returns true if an update was performed.
bool SyncBuffer::UpdateRoundMapDisplay(VoxelMap& voxelMap)
{
    if (roundMapUpdatedVisuals)
    {
        // Note that because we don't 'unset' the boolean and there's only one reader,
        //  we don't need to check the if-block again.
        ReadLock readLock(mapUpdateMutex);
        voxelMap.SetupFromMap(gameRound.map);
        roundMapUpdatedVisuals = false;

        return true;
    }

    return false;
}

// Updates the view matrix given the viewer position and rotation.
void SyncBuffer::UpdateViewMatrix(const vec::vec3& position, const vec::quaternion& rotation)
{
    WriteLock writeLock(viewMatrixMutex);
    viewMatrix = rotation.asMatrix() * MatrixOps::Translate(-position);
}

// Retrives a copy of the current view matrix.
vec::mat4 SyncBuffer::GetViewMatrix()
{
    vec::mat4 copy;

    ReadLock readLock(viewMatrixMutex);
    copy = viewMatrix;
    return copy;
}

void SyncBuffer::UpdateViewerPosition(const vec::vec3& viewerPos)
{
    WriteLock writeLock(viewerPositionMutex);
    viewerPosition = viewerPos;
}

vec::vec3 SyncBuffer::GetViewerPosition()
{
    vec::vec3 copy;
    ReadLock readLock(viewerPositionMutex);
    copy = viewerPosition;

    return copy;
}

// Sets a new voxel as the selected voxel.
void SyncBuffer::SetNewSelectedVoxel(const vec::vec3i& selectedVoxel)
{
    WriteLock writeLock(selectedVoxelMutex);
    this->selectedVoxel = selectedVoxel;
    newSelectedVoxel = true;
}

// Attempts to get a new selected voxel. If successful, returns true (and will return false subsequently until the next update).
bool SyncBuffer::TryGetNewSelectedVoxel(vec::vec3i* selectedVoxel)
{
    if (newSelectedVoxel)
    {
        // Note that because we don't 'unset' the boolean and there's only one reader,
        //  we don't need to check the if-block again.
        ReadLock readLock(selectedVoxelMutex);
        *selectedVoxel = this->selectedVoxel;
        newSelectedVoxel = false;

        return true;
    }

    return false;
}