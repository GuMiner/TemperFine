#pragma once
#include "GameRound.h"
#include "MapSections.h"
#include "ModelManager.h"
#include "RouteVisual.h"
#include "SharedExclusiveLock.h"
#include "Unit.h"
#include "Vec.h"
#include "VoxelMap.h"

// Acts as a buffer between the physics system and GUI system, to enforce single-threaded access to shared resources
class SyncBuffer
{
public:
    SyncBuffer();

    // Adds the player to the players in the round.
    void AddPlayer(const std::string& playerName);

    // Renders the players
    void RenderPlayers(ModelManager& modelManager, RouteVisual& routeVisuals, vec::mat4& projectionMatrix);

    // TODO more fine-grained semaphore concurrency
    void AddUnit(unsigned int playerId, const Unit& unit);
    void UpdatePlayers();

    // Sets the round map.
    void SetRoundMap(const MapInfo& testMap);

    // Updates the round map physics. Returns true if an update was performed.
    bool UpdateRoundMapPhysics(MapSections& mapSections);

    // Updates the round map display. Returns true if an update was performed.
    bool UpdateRoundMapDisplay(VoxelMap& voxelMap);

    // Updates the view matrix given the viewer position and rotation.
    void UpdateViewMatrix(const vec::vec3& position, const vec::quaternion& rotation);

    // Retrives a copy of the current view matrix.
    vec::mat4 GetViewMatrix();

    // Sets a new voxel as the selected voxel.
    void SetNewSelectedVoxel(const vec::vec3i& selectedVoxel);

    // Attempts to get a new selected voxel. If successful, returns true (and will return false subsequently until the next update).
    bool TryGetNewSelectedVoxel(vec::vec3i* selectedVoxel);

private:
    GameRound gameRound;

    // Mutex for updating the player vector. Acquire a WriteLock to add/remove players.
    SharedExclusiveLock playerVectorMutex;
    
    SharedExclusiveLock mapUpdateMutex;
    bool roundMapUpdatedVisuals;
    bool roundMapUpdatedPhysics;

    // View matrix and operation mutex.
    SharedExclusiveLock viewMatrixMutex;
    vec::mat4 viewMatrix;

    // If there's a new selected voxel.
    SharedExclusiveLock selectedVoxelMutex;
    bool newSelectedVoxel;
    vec::vec3i selectedVoxel;
};