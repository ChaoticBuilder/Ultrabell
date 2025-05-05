#ifndef HUD_H
#define HUD_H

#include <PR/ultratypes.h>
#include <PR/gbi.h>

#define HUD_POWER_METER_X            152
#define HUD_POWER_METER_EMPHASIZED_Y 164
#define HUD_POWER_METER_Y            202
#define HUD_POWER_METER_HIDDEN_Y     266

#define HUD_TOP_Y 212
#define HUD_BOTTOM_Y 9

// #define HUD_COINS_X 168
#define HUD_STARS_X 84 // This starts counting from the right edge
// #define HUD_CAMERA_X 54 // This starts counting from the right edge

extern s32 gHudShakeX;
extern s32 gHudShakeY;
extern u8 troll;
extern u8 debugScroll;
extern u16 musicID;
extern u16 musicBank;
extern u8 pitchInvert;

#ifdef TIME_ATTACK
    extern int timeATKCount;
    extern int timeATKCount2;
    extern int timeATKSecs;
    extern int timeATKMins;
#endif

enum PowerMeterAnimation {
    POWER_METER_HIDDEN,
    POWER_METER_EMPHASIZED,
    POWER_METER_DEEMPHASIZING,
    POWER_METER_HIDING,
    POWER_METER_VISIBLE
};

#ifdef BREATH_METER
enum BreathMeterAnimation {
    BREATH_METER_HIDDEN,
    BREATH_METER_SHOWING,
    BREATH_METER_HIDING,
    BREATH_METER_VISIBLE
};
#endif

/* DISABLED FEATURE:
enum CameraHUDLUT {
    GLYPH_CAM_CAMERA,
    GLYPH_CAM_MARIO_HEAD,
    GLYPH_CAM_LAKITU_HEAD,
    GLYPH_CAM_FIXED,
    GLYPH_CAM_ARROW_UP,
    GLYPH_CAM_ARROW_DOWN
};
*/

// void set_hud_camera_status(s16 status);
void render_hud(void);

#endif // HUD_H
