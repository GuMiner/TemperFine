#pragma once
#include "GameRound.h"
#include "MapSections.h"
#include "ModelManager.h"
#include "RouteVisual.h"
#include "SharedExclusiveLock.h"
#include "Unit.h"
#include "UnitRouter.h"
#include "Vec.h"
#include "VoxelMap.h"

// Acts as a buffer between the physics system and GUI system, to enforce single-threaded access to shared resources
class SyncBuffer
{
public:
    SyncBuffer();

    // Adds the player to the players in the round.
    void AddPlayer(const std::string& playerName);

    // Locks a player for direct physics-thread use.
    Player& LockPlayer(unsigned int playerId);

    // Unlocks a player for direct physics-thread use.
    void UnlockPlayer(unsigned int playerId);

    // Renders the players
    void RenderPlayers(ModelManager& modelManager, RouteVisual& routeVisuals, vec::mat4& projectionMatrix);

    // Updates the players.
    void UpdatePlayers(float lastElapsedTime);

    // Sets the round map.
    void SetRoundMap(const MapInfo& testMap);

    // Updates the round map physics. Returns true if an update was performed.
    bool UpdateRoundMapPhysics(MapSections& mapSections);

    // Wrapper to MapSections HitByRay method, locking and providing the map.
    bool HitByRay(MapSections& mapSections, const vec::vec3& rayStart, const vec::vec3& rayVector, vec::vec3i* voxelId);

    // Wrapper to UnitRouter's RefineRoute, locking and providing the map.
    void RefineRoute(UnitRouter& unitRouter, const voxelSubsectionsMap& voxelSubsections, const vec::vec3i start, const vec::vec3i destination,
        const std::vector<vec::vec3i>& givenPath, std::vector<vec::vec3i>& refinedPath, std::vector<vec::vec3>& visualPath);

    // Updates the round map display. Returns true if an update was performed.
    bool UpdateRoundMapDisplay(VoxelMap& voxelMap);

    // Updates the view matrix given the viewer position and rotation.
    void UpdateViewMatrix(const vec::vec3& position, const vec::quaternion& rotation);

    // Retrives a copy of the current view matrix.
    vec::mat4 GetViewMatrix();

    // Gets and sets the viewer position.
    void UpdateViewerPosition(const vec::vec3& viewerPos);
    vec::vec3 GetViewerPosition();

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

    SharedExclusiveLock viewerPositionMutex;
    vec::vec3 viewerPosition;

    // If there's a new selected voxel.
    SharedExclusiveLock selectedVoxelMutex;
    bool newSelectedVoxel;
    vec::vec3i selectedVoxel;
};