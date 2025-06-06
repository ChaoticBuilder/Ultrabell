#ifndef OBJECT_LIST_PROCESSOR_H
#define OBJECT_LIST_PROCESSOR_H

#include <PR/ultratypes.h>

#include "area.h"
#include "macros.h"
#include "types.h"

/**
 * Flags for gTimeStopState. These control which objects are processed each frame
 * and also track some miscellaneous info.
 */
enum gTimeStopStateFlags {
    TIME_STOP_UNKNOWN_0         = (1 << 0),
    TIME_STOP_ENABLED           = (1 << 1),
    TIME_STOP_DIALOG            = (1 << 2),
    TIME_STOP_MARIO_AND_DOORS   = (1 << 3),
    TIME_STOP_ALL_OBJECTS       = (1 << 4),
    TIME_STOP_MARIO_OPENED_DOOR = (1 << 5),
    TIME_STOP_ACTIVE            = (1 << 6),
};

/**
 * The maximum number of objects that can be loaded at once.
 */
#define OBJECT_POOL_CAPACITY 240

/**
 * Every object is categorized into an object list, which controls the order
 * they are processed and which objects they can collide with.
 */
enum ObjectList {
    OBJ_LIST_PLAYER,      //  (0) Mario
    OBJ_LIST_UNUSED_1,    //  (1) (unused)
    OBJ_LIST_DESTRUCTIVE, //  (2) things that can be used to destroy other objects, like
                          //      bob-ombs and corkboxes
    OBJ_LIST_UNUSED_3,    //  (3) (unused)
    OBJ_LIST_GENACTOR,    //  (4) general actors. most normal 'enemies' or actors are
                          //      on this list. (MIPS, bullet bill, bully, etc)
    OBJ_LIST_PUSHABLE,    //  (5) pushable actors. This is a group of objects which
                          //      can push each other around as well as their parent
                          //      objects. (goombas, koopas, spinies)
    OBJ_LIST_LEVEL,       //  (6) level objects. general level objects such as heart, star
    OBJ_LIST_UNUSED_7,    //  (7) (unused)
    OBJ_LIST_DEFAULT,     //  (8) default objects. objects that didnt start with a 00
                          //      command are put here, so this is treated as a default.
    OBJ_LIST_SURFACE,     //  (9) surface objects. objects that specifically have surface
                          //      collision and not object collision. (thwomp, whomp, etc)
    OBJ_LIST_POLELIKE,    // (10) polelike objects. objects that attract or otherwise
                          //      "cling" Mario similar to a pole action. (hoot,
                          //      whirlpool, trees/poles, etc)
    OBJ_LIST_SPAWNER,     // (11) spawners
    OBJ_LIST_UNIMPORTANT, // (13) unimportant objects. objects that will not load
                          //      if there are not enough object slots: they will also
                          //      be manually unloaded to make room for slots if the list
                          //      gets exhausted.
    NUM_OBJ_LISTS
};


extern struct ObjectNode gObjectListArray[];

extern s32 gDebugInfoFlags;
extern s32 gNumFindFloorMisses;
extern s32 gUnknownWallCount;
extern u32 gObjectCounter;
extern u32 gUnimportantCounter;

struct NumTimesCalled {
    /*0x00*/ s16 floor;
    /*0x02*/ s16 ceil;
    /*0x04*/ s16 wall;
};

extern struct NumTimesCalled gNumCalls;

extern s16 gDebugInfo[][8];
extern s16 gDebugInfoOverwrite[][8];

extern u32 gTimeStopState;
extern struct Object gObjectPool[];
extern struct Object gMacroObjectDefaultParent;
extern struct ObjectNode *gObjectLists;
extern struct ObjectNode gFreeObjectList;

extern struct Object *gMarioObject;
extern struct Object *gLuigiObject;
extern struct Object *gCurrentObject;
#define o gCurrentObject

extern const BehaviorScript *gCurBhvCommand;
extern s16 gPrevFrameObjectCount;

extern s32 gSurfaceNodesAllocated;
extern s32 gSurfacesAllocated;
extern s32 gNumStaticSurfaceNodes;
extern s32 gNumStaticSurfaces;

#define OBJECT_MEMORY_POOL 0x800

extern struct MemoryPool *gObjectMemoryPool;

enum CollisionFlags {
    COLLISION_FLAGS_NONE              = (0 << 0),
    COLLISION_FLAG_RETURN_FIRST       = (1 << 1),
    COLLISION_FLAG_CAMERA             = (1 << 2),
    COLLISION_FLAG_INCLUDE_INTANGIBLE = (1 << 3),
    COLLISION_FLAG_EXCLUDE_DYNAMIC    = (1 << 4),
};

extern s16 gCollisionFlags;

extern TerrainData *gEnvironmentRegions;
extern s32 gEnvironmentLevels[20];

/**
 * The maximum number of door/transition rooms that load two rooms of objects at once.
 */
#define MAX_NUM_TRANSITION_ROOMS 60

struct TransitionRoomData {
    /*0x00*/ RoomData forwardRoom;
    /*0x01*/ RoomData backwardRoom;
}; /*0x02*/

extern struct TransitionRoomData gDoorAdjacentRooms[MAX_NUM_TRANSITION_ROOMS];

extern s16 gMarioCurrentRoom;
extern s16 gTHIWaterDrained;
extern s16 gTTCSpeedSetting;
extern s16 gMarioShotFromCannon;
extern s16 gCCMEnteredSlide;
extern s16 gNumRoomedObjectsInMarioRoom;
extern s16 gNumRoomedObjectsNotInMarioRoom;
extern s16 gWDWWaterLevelChanging;
extern s16 gMarioOnMerryGoRound;


void bhv_mario_update(void);
void set_object_respawn_info_bits(struct Object *obj, u8 bits);
void unload_objects_from_area(UNUSED s32 unused, s32 areaIndex);
void spawn_objects_from_info(UNUSED s32 unused, struct SpawnInfo *spawnInfo);
void clear_objects(void);
void clear_dynamic_surface_references(void);
void update_objects(UNUSED s32 unused);


#endif // OBJECT_LIST_PROCESSOR_H
