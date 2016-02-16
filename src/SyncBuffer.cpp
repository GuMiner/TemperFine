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
    playerVectorMutex.lock();
    gameRound.players.push_back(Player(playerName, (int)gameRound.players.size()));
    playerVectorMutex.unlock();
}

void SyncBuffer::RenderPlayers(ModelManager& modelManager, RouteVisual& routeVisuals, vec::mat4& projectionMatrix)
{
    playerVectorMutex.lock();
    for (unsigned int i = 0; i < gameRound.players.size(); i++)
    {
        gameRound.players[i].RenderUnits(modelManager, routeVisuals, projectionMatrix);
    }
    playerVectorMutex.unlock();
}

// Sets the round map.
void SyncBuffer::SetRoundMap(const MapInfo& testMap)
{
    mapUpdateMutex.lock();
    gameRound.map = testMap;
    roundMapUpdatedVisuals = true;
    roundMapUpdatedPhysics = true;
    mapUpdateMutex.unlock();
}

bool SyncBuffer::UpdateRoundMapPhysics(MapSections& mapSections)
{
    // TODO this should use a semaphore (which we'd implement) instead.
    if (roundMapUpdatedPhysics)
    {
        // Note that because we don't 'unset' the boolean and there's only one reader,
        //  we don't need to check the if-block again.
        mapUpdateMutex.lock();
        mapSections.RecomputeMapSections(gameRound.map);
        roundMapUpdatedPhysics = false;
        mapUpdateMutex.unlock();

        return true;
    }

    return false;
}

// Updates the round map display. Returns true if an update was performed.
bool SyncBuffer::UpdateRoundMapDisplay(VoxelMap& voxelMap)
{
    if (roundMapUpdatedVisuals)
    {
        // Note that because we don't 'unset' the boolean and there's only one reader,
        //  we don't need to check the if-block again.
        mapUpdateMutex.lock();
        voxelMap.SetupFromMap(gameRound.map);
        roundMapUpdatedVisuals = false;
        mapUpdateMutex.unlock();

        return true;
    }

    return false;
}

// Updates the view matrix given the viewer position and rotation.
void SyncBuffer::UpdateViewMatrix(const vec::vec3& position, const vec::quaternion& rotation)
{
    viewMatrixMutex.lock();
    viewMatrix = rotation.asMatrix() * MatrixOps::Translate(-position);
    viewMatrixMutex.unlock();
}

// Retrives a copy of the current view matrix.
vec::mat4 SyncBuffer::GetViewMatrix()
{
    vec::mat4 copy;

    viewMatrixMutex.lock();
    copy = viewMatrix;
    viewMatrixMutex.unlock();

    return copy;
}

// Sets a new voxel as the selected voxel.
void SyncBuffer::SetNewSelectedVoxel(const vec::vec3i& selectedVoxel)
{
    selectedVoxelMutex.lock();
    this->selectedVoxel = selectedVoxel;
    newSelectedVoxel = true;
    selectedVoxelMutex.unlock();
}

// Attempts to get a new selected voxel. If successful, returns true (and will return false subsequently until the next update).
bool SyncBuffer::TryGetNewSelectedVoxel(vec::vec3i* selectedVoxel)
{
    if (newSelectedVoxel)
    {
        // Note that because we don't 'unset' the boolean and there's only one reader,
        //  we don't need to check the if-block again.
        selectedVoxelMutex.lock();
        *selectedVoxel = this->selectedVoxel;
        newSelectedVoxel = false;
        selectedVoxelMutex.unlock();

        return true;
    }

    return false;
}