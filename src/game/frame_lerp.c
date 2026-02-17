#include "types.h"
#include "engine/math_util.h"
#include "frame_lerp.h"
#include "main.h"
#include "game_init.h"
#include <PR/os_internal_reg.h>

u32 gFrameLerpRenderFrame;
f32 gFrameLerpDeltaTime;
f32 gDeltaProgress = 0.0f;

f32 * frameLerpPos(Vec3f pos, Vec3f lerpValue) {
    f32 dsq; vec3f_get_dist_squared(pos,lerpValue,&dsq);
    if (dsq > 160000.f) {
        vec3f_copy(lerpValue,pos);
        return lerpValue;
    }

    if (gFrameLerpRenderFrame == FRAMELERP_NORMAL) {
        vec3f_copy(lerpValue,pos);
    } else {
        Vec3f inBetween;
        for (int i = 0; i<3; i++) {
            inBetween[i] = pos[i] + (lerpValue[i] - pos[i])*.5f;
        }
        vec3f_copy(lerpValue,inBetween);
    }
    return lerpValue;
}

f32 * frameLerpRot(Quat rot, Quat lerpValue) {
    if (2.0*acosf(absf(quat_dot(rot,lerpValue)))>2.f) {
        quat_copy(lerpValue,rot);
        return lerpValue;
    }
    if (gFrameLerpRenderFrame == FRAMELERP_NORMAL) {
        quat_copy(lerpValue,rot);
    } else {
        Quat inBetween;
        quat_slerp(inBetween,rot,lerpValue,0.5f);
        quat_copy(lerpValue,inBetween);
    }
    return lerpValue;
}

f32 frameLerpFloat(f32 f, f32 lerpValue) {
    if (gFrameLerpRenderFrame == FRAMELERP_NORMAL) {
        return f;
    } else {
        return f + (lerpValue - f)*.5f;
    }
    return lerpValue;
}

f32 * sCachedPosUpdateRealList[1000];
f32 * sCachedPosUpdateCacheList[1000];
f32 * sCachedPosUpdateCacheVideoList[1000];
int sCachedPosCt = 0;
int sCachePosTotal = 0;

void frameLerp_cache_pos(f32 * realPosPtr, f32 * cachePosPtr, f32 * cachePosVideoPtr) {
    sCachedPosUpdateRealList[sCachedPosCt] = realPosPtr;
    sCachedPosUpdateCacheList[sCachedPosCt] = cachePosPtr;
    sCachedPosUpdateCacheVideoList[sCachedPosCt] = cachePosVideoPtr;
    sCachedPosCt++;
}

void frameLerp_update_pos_cache(void) {
    u32 mask = __osDisableInt();
    for (int i = 0; i < sCachedPosCt; i++) {
        vec3f_copy( sCachedPosUpdateCacheList[i], sCachedPosUpdateRealList[i] );
    }
    sCachePosTotal = sCachedPosCt;
    sCachedPosCt = 0;
    __osRestoreInt(mask);
}

void frameLerp_update_pos_video_cache(void) {
    u32 mask = __osDisableInt();
    for (int i = 0; i < sCachePosTotal; i++) {
        vec3f_copy( sCachedPosUpdateCacheVideoList[i], sCachedPosUpdateCacheList[i] );
    }
    __osRestoreInt(mask);
}