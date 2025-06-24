#ifndef HUD_H
#define HUD_H

#include <PR/ultratypes.h>
#include <PR/gbi.h>

#define HUD_TOP_Y (SCREEN_HEIGHT - 28) /* 12px from top */
#define HUD_BOTTOM_Y 9

#define HUD_POWER_METER_X            42 /* Counts from the right edge*/
#define HUD_POWER_METER_EMPHASIZED_Y (HUD_TOP_Y - 42)
#define HUD_POWER_METER_Y            (HUD_TOP_Y - 9)
#define HUD_POWER_METER_HIDDEN_Y     (HUD_TOP_Y + 55)

#define HUD_STATS_X 56

extern s16 gHudShakeX, gHudShakeY;
extern u8 pitchInvert;
extern u8 sleepTimer;

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

void render_hud(void);

#endif // HUD_H
