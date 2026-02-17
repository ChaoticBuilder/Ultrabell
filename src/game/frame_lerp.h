#include "types.h"

#ifndef FRAME_LERP_H
#define FRAME_LERP_H

extern u32 gFrameLerpRenderFrame;
extern f32 gFrameLerpDeltaTime;

enum enumFrameLerpRenderFrame {
    FRAMELERP_NORMAL,
    FRAMELERP_BETWEEN,
    FRAMELERP_SLOW
};

f32 * frameLerpPos(Vec3f pos, Vec3f lerpValue);
f32 * frameLerpRot(Quat rot, Quat rotValue);
f32 frameLerpFloat(f32 f, f32 lerpValue);

void frameLerp_cache_pos(f32 * realPosPtr, f32 * cachePosPtr, f32 * cachePosVideoPtr);
void frameLerp_update_pos_cache(void);
void frameLerp_update_pos_video_cache(void);


#endif