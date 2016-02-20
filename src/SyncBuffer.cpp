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

void SyncBuffer::AddUnit(unsigned int playerId, const Unit& unit)
{
    ReadLock readLock(playerVectorMutex);
    if (playerId < gameRound.players.size())
    {
        gameRound.players[playerId].AddUnit(unit);
    }
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