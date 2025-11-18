#include <PR/ultratypes.h>

#include "sm64.h"
#include "actors/common1.h"
#include "gfx_dimensions.h"
#include "game_init.h"
#include "level_update.h"
#include "camera.h"
#include "print.h"
#include "ingame_menu.h"
#include "hud.h"
#include "segment2.h"
#include "area.h"
#include "save_file.h"
#include "engine/surface_load.h"
#include "engine/math_util.h"
#include "puppycam2.h"
#include "puppyprint.h"
#include "menu/star_select.h"
#include "main.h"
#include "object_list_processor.h"
#include "sound_init.h"
#include "audio/external.h"
#include "audio/load.h"
#include "audio/internal.h"
#include "audio/heap.h"
#include "audio/seqplayer.h"
#include "emutest.h"

#include "config.h"

u8 gLuigiToggle = FALSE;
s32 gSecondsToggle = TRUE;
u8 gHudToggle;
u8 pitchInvert = 1;

/* @file hud.c
 * This file implements HUD rendering and power meter animations.
 * That includes stars, lives, coins, camera status, power meter, timer
 * cannon reticle, and the unused keys.
 **/

#ifdef BREATH_METER
#define HUD_BREATH_METER_X         40
#define HUD_BREATH_METER_Y         32
#define HUD_BREATH_METER_HIDDEN_Y -20
#endif

// ------------- FPS COUNTER ---------------
// To use it, call print_fps(x,y); every frame.
#define FRAMETIME_COUNT 30

OSTime frameTimes[FRAMETIME_COUNT];
u8 curFrameTimeIndex = 0;

#include "PR/os_convert.h"

f32 gDeltaTime;

#ifdef USE_PROFILER
float profiler_get_fps();
#else
// Call once per frame
f32 calculate_and_update_fps() {
    OSTime newTime = osGetTime();
    OSTime oldTime = frameTimes[curFrameTimeIndex];
    frameTimes[curFrameTimeIndex] = newTime;

    curFrameTimeIndex++;
    if (curFrameTimeIndex >= FRAMETIME_COUNT) {
        curFrameTimeIndex = 0;
    }
    return ((f32)FRAMETIME_COUNT * 1000000.0f) / (s32)OS_CYCLES_TO_USEC(newTime - oldTime);
}
#endif

void fps_calc(void) {
    #ifdef USE_PROFILER
        gDeltaTime = profiler_get_fps();
    #else
        gDeltaTime = calculate_and_update_fps();
    #endif
    gDeltaTime /= 30.0f;
}

void print_fps(s32 x, s32 y) {
    char text[14];

    sprintf(text, "FPS %2.3f", gDeltaTime);
#ifdef PUPPYPRINT
    print_small_text(x, y, text, PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_OUTLINE);
#else
    print_text(x, y, text);
#endif
}

// ------------ END OF FPS COUNER -----------------

struct PowerMeterHUD {
    s8 animation;
    s16 x;
    s16 y;
};

// Stores health segmented value defined by numHealthWedges
// When the HUD is rendered this value is 8, full health.
static s16 sPowerMeterStoredHealth;

static struct PowerMeterHUD sPowerMeterHUD = {
    POWER_METER_HIDDEN,
    HUD_POWER_METER_X,
    HUD_POWER_METER_HIDDEN_Y,
};

// Power Meter timer that keeps counting when it's visible.
// Gets reset when the health is filled and stops counting
// when the power meter is hidden.
u8 sPowerMeterVisibleTimer = 0;

#ifdef BREATH_METER
static s16 sBreathMeterStoredValue;
static struct PowerMeterHUD sBreathMeterHUD = {
    BREATH_METER_HIDDEN,
    HUD_BREATH_METER_X,
    HUD_BREATH_METER_HIDDEN_Y,
};
s32 sBreathMeterVisibleTimer = 0;
#endif

s16 gHudShakeX, gHudShakeY;

/**
 * Renders a rgba16 16x16 glyph texture from a table list.
 */
void render_hud_tex_lut(s32 x, s32 y, Texture *texture) {
    Gfx *tempGfxHead = gDisplayListHead;

    gDPPipeSync(tempGfxHead++);
    gDPSetTextureImage(tempGfxHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture);
    gSPDisplayList(tempGfxHead++, &dl_hud_img_load_tex_block);
    gSPTextureRectangle(tempGfxHead++, x << 2, y << 2, (x + 15) << 2, (y + 15) << 2,
                        G_TX_RENDERTILE, 0, 0, 4 << 10, 1 << 10);

    gDisplayListHead = tempGfxHead;
}

/**
 * Renders a rgba16 8x8 glyph texture from a table list.
 */
void render_hud_small_tex_lut(s32 x, s32 y, Texture *texture) {
    Gfx *tempGfxHead = gDisplayListHead;

    gDPSetTile(tempGfxHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0,
                G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD);
    gDPTileSync(tempGfxHead++);
    gDPSetTile(tempGfxHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 2, 0, G_TX_RENDERTILE, 0,
                G_TX_CLAMP, 3, G_TX_NOLOD, G_TX_CLAMP, 3, G_TX_NOLOD);
    gDPSetTileSize(tempGfxHead++, G_TX_RENDERTILE, 0, 0, (8 - 1) << G_TEXTURE_IMAGE_FRAC, (8 - 1) << G_TEXTURE_IMAGE_FRAC);
    gDPPipeSync(tempGfxHead++);
    gDPSetTextureImage(tempGfxHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture);
    gDPLoadSync(tempGfxHead++);
    gDPLoadBlock(tempGfxHead++, G_TX_LOADTILE, 0, 0, 8 * 8 - 1, CALC_DXT(8, G_IM_SIZ_16b_BYTES));
    gSPTextureRectangle(tempGfxHead++, x << 2, y << 2, (x + 7) << 2, (y + 7) << 2, G_TX_RENDERTILE,
                        0, 0, 4 << 10, 1 << 10);

    gDisplayListHead = tempGfxHead;
}

/**
 * Renders power meter health segment texture using a table list.
 */
void render_power_meter_health_segment(s16 numHealthWedges) {
    Texture *(*healthLUT)[] = segmented_to_virtual(&power_meter_health_segments_lut);
    Gfx *tempGfxHead = gDisplayListHead;

    gDPPipeSync(tempGfxHead++);
    gDPSetTextureImage(tempGfxHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1,
                       (*healthLUT)[numHealthWedges - 1]);
    gDPLoadSync(tempGfxHead++);
    gDPLoadBlock(tempGfxHead++, G_TX_LOADTILE, 0, 0, 32 * 32 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES));
    gSP1Triangle(tempGfxHead++, 0, 1, 2, 0);
    gSP1Triangle(tempGfxHead++, 0, 2, 3, 0);

    gDisplayListHead = tempGfxHead;
}

/**
 * Renders power meter display lists.
 * That includes the "POWER" base and the colored health segment textures.
 */
void render_dl_power_meter(s16 numHealthWedges) {
    Mtx *mtx = alloc_display_list(sizeof(Mtx));

    if (mtx == NULL) {
        return;
    }

    guTranslate(mtx, (f32) GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(sPowerMeterHUD.x) + gHudShakeX, (f32) sPowerMeterHUD.y  + gHudShakeY, 0);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(mtx++),
              G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPDisplayList(gDisplayListHead++, &dl_power_meter_base);

    if (numHealthWedges != 0) {
        gSPDisplayList(gDisplayListHead++, &dl_power_meter_health_segments_begin);
        render_power_meter_health_segment(numHealthWedges);
        gSPDisplayList(gDisplayListHead++, &dl_power_meter_health_segments_end);
    }

    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

/**
 * Power meter animation called when there's less than 8 health segments
 * Checks its timer to later change into deemphasizing mode.
 */
void animate_power_meter_emphasized(void) {
    s16 speed = 1 + (((sPowerMeterHUD.y - HUD_POWER_METER_EMPHASIZED_Y) + 1) / 2);
    
    if (vBlankTimer) approach_s16_symmetric_bool(&sPowerMeterHUD.y, HUD_POWER_METER_EMPHASIZED_Y, speed);
    if (sPowerMeterVisibleTimer >= 30) {
        sPowerMeterHUD.animation = POWER_METER_DEEMPHASIZING;
        sPowerMeterVisibleTimer = 0;
    }
}

/**
 * Power meter animation called after emphasized mode.
 * Moves power meter y pos speed until it's at 200 to be visible.
 */
static void animate_power_meter_deemphasizing(void) {
    s16 speed = 1 + (((HUD_POWER_METER_Y - sPowerMeterHUD.y) + 1) / 6);

    if (vBlankTimer) approach_s16_symmetric_bool(&sPowerMeterHUD.y, HUD_POWER_METER_Y, speed);
    if (sPowerMeterHUD.y > HUD_POWER_METER_Y) {
        sPowerMeterHUD.y = HUD_POWER_METER_Y;
        sPowerMeterHUD.animation = POWER_METER_VISIBLE;
    }
}

/**
 * Power meter animation called when there's 8 health segments.
 * Moves power meter y pos quickly until it's at 301 to be hidden.
 */
static void animate_power_meter_hiding(void) {
    if (vBlankTimer) approach_s16_symmetric_bool(&sPowerMeterHUD.y, HUD_POWER_METER_HIDDEN_Y, sqr(sPowerMeterVisibleTimer));
    if (sPowerMeterHUD.y >= HUD_POWER_METER_HIDDEN_Y) {
        sPowerMeterHUD.animation = POWER_METER_HIDDEN;
        sPowerMeterVisibleTimer = 0;
    }
}

/**
 * Handles power meter actions depending of the health segments values.
 */
void handle_power_meter_actions(s16 numHealthWedges) {
    // Show power meter if health is not full, less than 8
    if (numHealthWedges < 8 && sPowerMeterStoredHealth == 8
        && sPowerMeterHUD.animation == POWER_METER_HIDDEN) {
        sPowerMeterHUD.animation = POWER_METER_EMPHASIZED;
        sPowerMeterHUD.y = HUD_POWER_METER_HIDDEN_Y;
        sPowerMeterVisibleTimer = 0;
    }

    // Show power meter if health is full, has 8
    if (numHealthWedges == 8 && sPowerMeterStoredHealth < 8) {
        sPowerMeterVisibleTimer = 0;
    }

    // After health is full, hide power meter
    if (numHealthWedges == 8 && sPowerMeterVisibleTimer >= 30) {
        sPowerMeterHUD.animation = POWER_METER_HIDING;
        sPowerMeterVisibleTimer = 3;
    }

    // Update to match health value
    sPowerMeterStoredHealth = numHealthWedges;

#ifndef BREATH_METER
    // If Mario is swimming, keep power meter visible
    if (gPlayerCameraState->action & ACT_FLAG_SWIMMING) {
        if (sPowerMeterHUD.animation == POWER_METER_HIDDEN
            || sPowerMeterHUD.animation == POWER_METER_EMPHASIZED) {
            sPowerMeterHUD.animation = POWER_METER_DEEMPHASIZING;
            sPowerMeterHUD.y = HUD_POWER_METER_EMPHASIZED_Y;
        }
        sPowerMeterVisibleTimer = 0;
    }
#endif
}

/**
 * Renders the power meter that shows when Mario is in underwater
 * or has taken damage and has less than 8 health segments.
 * And calls a power meter animation function depending of the value defined.
 */
void render_hud_power_meter(void) {
    s16 shownHealthWedges = gHudDisplay.wedges;
    if (sPowerMeterHUD.animation != POWER_METER_HIDING) handle_power_meter_actions(shownHealthWedges);
    if (sPowerMeterHUD.animation == POWER_METER_HIDDEN) return;
    switch (sPowerMeterHUD.animation) {
        case POWER_METER_EMPHASIZED:    animate_power_meter_emphasized();    break;
        case POWER_METER_DEEMPHASIZING: animate_power_meter_deemphasizing(); break;
        case POWER_METER_HIDING:        animate_power_meter_hiding();        break;
        default:                                                             break;
    }
    render_dl_power_meter(shownHealthWedges);
    if (vBlankTimer) sPowerMeterVisibleTimer++;
}

#ifdef BREATH_METER
/**
 * Renders breath meter health segment texture using a table list.
 */
void render_breath_meter_segment(s16 numBreathWedges) {
    Texture *(*breathLUT)[];
    breathLUT = segmented_to_virtual(&breath_meter_segments_lut);
    Gfx *tempGfxHead = gDisplayListHead;

    gDPPipeSync(tempGfxHead++);
    gDPSetTextureImage(tempGfxHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, (*breathLUT)[numBreathWedges - 1]);
    gDPLoadSync(tempGfxHead++);
    gDPLoadBlock(tempGfxHead++, G_TX_LOADTILE, 0, 0, 32 * 32 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES));
    gSP1Triangle(tempGfxHead++, 0, 1, 2, 0);
    gSP1Triangle(tempGfxHead++, 0, 2, 3, 0);

    gDisplayListHead = tempGfxHead;
}

/**
 * Renders breath meter display lists.
 * That includes the base and the colored segment textures.
 */
void render_dl_breath_meter(s16 numBreathWedges) {
    Mtx *mtx = alloc_display_list(sizeof(Mtx));

    if (mtx == NULL) {
        return;
    }

    guTranslate(mtx, (f32) sBreathMeterHUD.x, (f32) sBreathMeterHUD.y, 0);
    gSPMatrix(      gDisplayListHead++, VIRTUAL_TO_PHYSICAL(mtx++),
                    G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPDisplayList( gDisplayListHead++, &dl_breath_meter_base);
    if (numBreathWedges != 0) {
        gSPDisplayList(gDisplayListHead++, &dl_breath_meter_health_segments_begin);
        render_breath_meter_segment(numBreathWedges);
        gSPDisplayList(gDisplayListHead++, &dl_breath_meter_health_segments_end);
    }
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

/**
 * Breath meter animation called after emphasized mode.
 * Moves breath meter y pos speed until it's visible.
 */
static void animate_breath_meter_sliding_in(void) {
    approach_s16_symmetric_bool(&sBreathMeterHUD.y, HUD_BREATH_METER_Y, 5);
    if (sBreathMeterHUD.y         == HUD_BREATH_METER_Y) {
        sBreathMeterHUD.animation = BREATH_METER_VISIBLE;
    }
}

/**
 * Breath meter animation called when there's 8 health segments.
 * Moves breath meter y pos quickly until it's hidden.
 */
static void animate_breath_meter_sliding_out(void) {
    approach_s16_symmetric_bool(&sBreathMeterHUD.y, HUD_BREATH_METER_HIDDEN_Y, 20);
    if (sBreathMeterHUD.y         == HUD_BREATH_METER_HIDDEN_Y) {
        sBreathMeterHUD.animation = BREATH_METER_HIDDEN;
    }
}

/**
 * Handles breath meter actions depending of the health segments values.
 */
void handle_breath_meter_actions(s16 numBreathWedges) {
    // Show breath meter if health is not full, less than 8
    if ((numBreathWedges < 8) && (sBreathMeterStoredValue == 8) && sBreathMeterHUD.animation == BREATH_METER_HIDDEN) {
        sBreathMeterHUD.animation = BREATH_METER_SHOWING;
        // sBreathMeterHUD.y         = HUD_BREATH_METER_Y;
    }
    // Show breath meter if breath is full, has 8
    if ((numBreathWedges == 8) && (sBreathMeterStoredValue  == 7)) sBreathMeterVisibleTimer  = 0;
    // After breath is full, hide breath meter
    if ((numBreathWedges == 8) && (sBreathMeterVisibleTimer > 45)) sBreathMeterHUD.animation = BREATH_METER_HIDING;
    // Update to match breath value
    sBreathMeterStoredValue = numBreathWedges;
    // If Mario is swimming, keep breath meter visible
    if (gPlayerCameraState->action & ACT_FLAG_SWIMMING) {
        if (sBreathMeterHUD.animation == BREATH_METER_HIDDEN) {
            sBreathMeterHUD.animation = BREATH_METER_SHOWING;
        }
        sBreathMeterVisibleTimer = 0;
    }
}

void render_hud_breath_meter(void) {
    s16 shownBreathAmount = gHudDisplay.breath;
    if (sBreathMeterHUD.animation != BREATH_METER_HIDING) handle_breath_meter_actions(shownBreathAmount);
    if (sBreathMeterHUD.animation == BREATH_METER_HIDDEN) return;
    switch (sBreathMeterHUD.animation) {
        case BREATH_METER_SHOWING:       animate_breath_meter_sliding_in();  break;
        case BREATH_METER_HIDING:        animate_breath_meter_sliding_out(); break;
        default:                                                             break;
    }
    render_dl_breath_meter(shownBreathAmount);
    sBreathMeterVisibleTimer++;
}
#endif

u8 statX = 0;
u16 hudStatsX = 0;
u8 addOffset = 0;
u8 hurtShake = FALSE;

u8 mTimer = 60;
u8 cTimer = 60;
u8 mTimerArg = 0;
u8 cTimerArg = 0;

void handle_stats(void) {
    u8 goal = (sPowerMeterHUD.animation == POWER_METER_DEEMPHASIZING ||
               sPowerMeterHUD.animation == POWER_METER_VISIBLE) ? 56 : 0;
    
    /* Star / Coin power of 10 increase */
    u8 addGoal = 0;
    if (ABS(gHudDisplay.stars) >= 100 || ABS(gHudDisplay.coins) >= 100) addGoal = 26;
    else if (ABS(gHudDisplay.stars) >= 10 || ABS(gHudDisplay.coins) >= 10) addGoal = 13;
    if (vBlankTimer && addOffset != addGoal) addOffset = approach_s16_symmetric(addOffset, addGoal, 3);

    /* Power Meter offset calculation */
    if (cTimerArg >= 2) statX = goal;
    else if (statX != goal) {
        cTimer = 1;
        u8 div = (goal > 0) ? 6 : 4;
        if (vBlankTimer) statX = approach_s16_symmetric(statX, goal, (div / 3 + ABS((goal - statX) + 1) / div));
    }
    hudStatsX = ABS(HUD_STATS_X) + statX + addOffset;

    if (gMarioState->hurtCounter) {
        hurtShake = TRUE;
    } else if (sPowerMeterHUD.animation > POWER_METER_EMPHASIZED) hurtShake = FALSE;
    if (hurtShake) mTimer = 1;
}

s16 mPosY = HUD_TOP_Y;
s16 cPosY = HUD_TOP_Y;

void ms_invis(void) {
    s16 speed = 1 + (ABS(mPosY - HUD_TOP_Y) / 3);
    if (mTimer > 0) mTimerArg = 0;
    switch (mTimerArg) {
        case 0:
            approach_s16_symmetric_bool(&mPosY, HUD_TOP_Y, speed);
            if (mTimer == 0) mTimerArg++;
            else mTimer--;
            break;
        case 1:
            if (mPosY < (SCREEN_HEIGHT + HUD_Y_SEPARATION)) approach_s16_symmetric_bool(&mPosY, (SCREEN_HEIGHT + HUD_Y_SEPARATION), speed);
            else mTimerArg++;
            break;
    }
}

void cs_invis(void) {
    s16 speed = 1 + (ABS(cPosY - HUD_TOP_Y) / 3);
    if (cTimer > 0) cTimerArg = 0;
    switch (cTimerArg) {
        case 0:
            approach_s16_symmetric_bool(&cPosY, HUD_TOP_Y, speed);
            if (cTimer == 0) cTimerArg++;
            else cTimer--;
            break;
        case 1:
            if (cPosY < (SCREEN_HEIGHT + HUD_Y_SEPARATION)) approach_s16_symmetric_bool(&cPosY, (SCREEN_HEIGHT + HUD_Y_SEPARATION), speed);
            else cTimerArg++;
            break;
    }
}

/**
 * Renders the amount of lives Mario has.
 */
void render_hud_mario_lives(void) {
    if (!hurtShake) {
        if (!gLuigiToggle)  print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(16) + gHudShakeX, mPosY + gHudShakeY, ","); // 'Mario' glyph
        else                print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(16) + gHudShakeX, mPosY + gHudShakeY, ";"); // 'Luigi' glyph
        print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(32) + gHudShakeX, mPosY + gHudShakeY, "*"); // 'X' glyph
        print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(48) + gHudShakeX, mPosY + gHudShakeY, "%d", gHudDisplay.lives);
    } else print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(16) + gHudShakeX, mPosY + gHudShakeY, "ROLA!");
}

#ifdef VANILLA_STYLE_CUSTOM_DEBUG
void render_debug_mode(void) {
    print_text(180, 40, "DEBUG MODE");
    print_text_fmt_int(5, 20, "Z %d", gMarioState->pos[2]);
    print_text_fmt_int(5, 40, "Y %d", gMarioState->pos[1]);
    print_text_fmt_int(5, 60, "X %d", gMarioState->pos[0]);
    print_text_fmt_int(10, 100, "SPD %d", (s32) gMarioState->forwardVel);
    print_text_fmt_int(10, 120, "ANG 0*%04x", (u16) gMarioState->faceAngle[1]);
    print_fps(10,80);
}
#endif

/**
 * Renders the amount of coins collected.
 */
void render_hud_coins(void) {
    if (gMarioState->numLives == MAX_NUM_LIVES) {
        // Thank you-a so much for-a playing my game!
        print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(hudStatsX + 32) + gHudShakeX, (HUD_TOP_Y - 17) + gHudShakeY, "1996");
    } else {
        print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(hudStatsX + 32) + gHudShakeX, (cPosY - HUD_Y_SEPARATION) + gHudShakeY, "$"); // 'Coin' glyph
        print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(hudStatsX + 16) + gHudShakeX, (cPosY - HUD_Y_SEPARATION) + gHudShakeY, "*"); // 'X' glyph
        print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(hudStatsX) + gHudShakeX, (cPosY - HUD_Y_SEPARATION) + gHudShakeY, "%d", gHudDisplay.coins);
    }
}

/**
 * Renders the amount of stars collected.
 * Disables "X" glyph when Mario has 100 stars or more.
 */
void render_hud_stars(void) {
    print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(hudStatsX + 32) + gHudShakeX, cPosY + gHudShakeY, "^"); // 'Star' glyph
    print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(hudStatsX + 16) + gHudShakeX, cPosY + gHudShakeY, "*"); // 'X' glyph
    print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(hudStatsX) + gHudShakeX, cPosY + gHudShakeY, "%d", gHudDisplay.stars);
    // print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(hudStatsX) + gHudShakeX, (HUD_TOP_Y - 33) + gHudShakeY, "100");
}

/**
 * Unused function that renders the amount of keys collected.
 * Leftover function from the beta version of the game.
 */
void render_hud_keys(void) {
    s16 i;

    for (i = 0; i < gHudDisplay.keys; i++) {
        print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE((i * 17) + 16) + gHudShakeX, (mPosY - HUD_Y_SEPARATION) + gHudShakeY, "|"); // unused glyph - beta key
    }
}

/**
 * Renders the timer when Mario start sliding in PSS.
 */
void render_hud_timer(void) {
    Texture *(*hudLUT)[58] = segmented_to_virtual(&main_hud_lut);
    u16 timerValFrames = gHudDisplay.timer;
    u16 timerMins = timerValFrames / (gDeltaTime * 1800.0f);
    u16 timerSecs = (timerValFrames - (timerMins * 1800)) / (gDeltaTime * 30.0f);
    u16 timerFracSecs = (timerValFrames / (u8)(gDeltaTime * 3.0f + 0.5f)) % 10;

#if MULTILANG
    switch (eu_get_language()) {
        case LANGUAGE_ENGLISH: print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(138), HUD_BOTTOM_Y,  "TIME"); break;
        case LANGUAGE_FRENCH:  print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(143), HUD_BOTTOM_Y, "TEMPS"); break;
        case LANGUAGE_GERMAN:  print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(138), HUD_BOTTOM_Y,  "ZEIT"); break;
    }
#else
    print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(138) + gHudShakeX, HUD_BOTTOM_Y + gHudShakeY, "TIME");
#endif

    print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(88) + gHudShakeX, HUD_BOTTOM_Y + gHudShakeY, "%0d", timerMins);
    print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(66) + gHudShakeX, HUD_BOTTOM_Y + gHudShakeY, "%02d", timerSecs);
    print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(32) + gHudShakeX, HUD_BOTTOM_Y + gHudShakeY, "%d", timerFracSecs);

    gSPDisplayList(gDisplayListHead++, dl_hud_img_begin);
    render_hud_tex_lut(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(77) + gHudShakeX, (HUD_TOP_Y + 4) + gHudShakeY, (*hudLUT)[GLYPH_APOSTROPHE]);
    render_hud_tex_lut(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(77) + gHudShakeX, (HUD_TOP_Y - 5) + gHudShakeY, (*hudLUT)[GLYPH_APOSTROPHE]);
    render_hud_tex_lut(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(43) + gHudShakeX, (HUD_TOP_Y + 4) + gHudShakeY, (*hudLUT)[GLYPH_APOSTROPHE]);
    gSPDisplayList(gDisplayListHead++, dl_hud_img_end);
}

int timerCount = 60;

void attack_timer(void) {
    if (!gTimerToggle) return;
    char timerBytes[32];

    int timerSecs = timerCount / 30;
    int timerMins = timerSecs / 60;

    if (gTimerToggle) {
        if (!(COURSE_IS_MAIN_COURSE(gCurrCourseNum))) {
            timerCount = gTimerTime * 30;
        } else {
            if (timerCount > 0) timerCount--;
            if (timerCount <= 0) {
                gMarioState->health -= 4;
                if (g95Toggle && !gRealToggle) gMarioState->health -= 1;
                if (!g95Toggle && gMarioState->action == ACT_PANTING) gMarioState->health -= 2;
                // pesky health regens -w-
            }
        }
    }
    sprintf(timerBytes, "TIME: %02d : %02d", timerMins, (timerSecs % 60));
    
    (timerSecs > 10)
    ? print_set_envcolour(0, 189, 255, 255)
    : print_set_envcolour(255, 0, 0, 255);
    print_small_text_light(160 + gHudShakeX, 200 + gHudShakeY, timerBytes,
    PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_OUTLINE);
}

u8 troll = FALSE;
void timer_troll(void) {
    f32 rand = random_float();
    if (((gGlobalTimer % 18000 == 0) && rand < 0.5f) /* || (gPlayer1Controller->buttonPressed == L_JPAD && !gConfigOpen) */) {
        troll = TRUE;
    }
    if (troll == TRUE) {
        if ((gGlobalTimer & 3) == 0) return;
        if (rand < 0.0625f) {
            troll = FALSE;
        }
        if (rand > 0.75f) return print_text(112, 120, "FIND THEM");
        return print_text(112, 120, "MARIO 64");
    }
}

u8 debugScroll = 1;
u16 musicID = 0;
u8 musicBank = 0xFF;

void music_menu_scroll(void) {
    if (!gMusicToggle || gConfigOpen) return;
    if (gGlobalTimer % 4 == 0) {
        if (gPlayer1Controller->buttonDown & U_JPAD) {
            debugScroll--;
            play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundSource);
        }
        if (gPlayer1Controller->buttonDown & D_JPAD) {
            debugScroll++;
            play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundSource);
        }
        if (gPlayer1Controller->buttonDown & L_JPAD) {
            if (debugScroll == 1) musicID--;
            if (debugScroll == 2) musicBank--;
            if (debugScroll == 4) pitchInvert--;
            if (debugScroll != 3) play_sound(SOUND_MENU_CLICK_CHANGE_VIEW, gGlobalSoundSource);
        }
        if (gPlayer1Controller->buttonDown & R_JPAD) {
            if (debugScroll == 1) musicID++;
            if (debugScroll == 2) musicBank++;
            if (debugScroll == 4) pitchInvert++;
            if (debugScroll != 3) play_sound(SOUND_MENU_CLICK_CHANGE_VIEW, gGlobalSoundSource);
        }
    }
    
    if (pitchInvert < 1) pitchInvert = 3;
    if (pitchInvert > 3) pitchInvert = 1;
    if (musicID == 0xFFFF) musicID = 0;
    if (musicBank == 0xFE) musicBank = 0xFF;
    if (debugScroll < 1) {
        debugScroll = 4;
    }
    if (debugScroll > 4) {
        debugScroll = 1;
    }
    if (gPlayer1Controller->buttonPressed & R_TRIG) {
        if (debugScroll == 1 || debugScroll == 2) {
            if (musicID == 0) stop_background_music(musicID);
            set_background_music(0, musicID, 0);
        }
        if (debugScroll == 3) {
            musicID = gAreas[gCurrAreaIndex].musicParam2;
            play_sound(SOUND_MENU_CLICK_CHANGE_VIEW, gGlobalSoundSource);
        }
    }
}

void music_menu(void) {
    if (!gMusicToggle || gConfigOpen) return;
    char currOption[64];
    music_menu_scroll();
    u8 yPos = 30;

    print_set_envcolour(206, 156, 255, 255);
    print_small_text_light(16, yPos, "SOUND TEST", PRINT_ALL, PRINT_ALL, FONT_OUTLINE);
    yPos += 12;

    print_set_envcolour(206, 156, 255, 255);
    print_small_text_light(16, yPos, "PRESS R BUTTON", PRINT_ALL, PRINT_ALL, FONT_OUTLINE);
    yPos += 16;

    print_set_envcolour(255, 132, 0, 255);
    if (debugScroll != 1) print_set_envcolour(189, 49, 115, 255);
    sprintf(currOption, "Play Sequence: d%02d  x%02x", musicID, musicID);

    print_small_text_light(16, yPos, currOption, PRINT_ALL, PRINT_ALL, FONT_OUTLINE);
    yPos += 12;

    print_set_envcolour(255, 132, 0, 255);
    if (debugScroll != 2) print_set_envcolour(189, 49, 115, 255);
    (musicBank != 0xFF)
    ? sprintf(currOption, "Instrument Set: d%02d  x%02x", musicBank, musicBank)
    : sprintf(currOption, "Instrument Set: Default");
    
    print_small_text_light(16, yPos, currOption, PRINT_ALL, PRINT_ALL, FONT_OUTLINE);
    yPos += 12;

    print_set_envcolour(255, 132, 0, 255);
    if (debugScroll != 3) print_set_envcolour(189, 49, 115, 255);
    
    print_small_text_light(16, yPos, "Reset Sequence", PRINT_ALL, PRINT_ALL, FONT_OUTLINE);
    yPos += 12;

    print_set_envcolour(255, 132, 0, 255);
    if (debugScroll != 4) print_set_envcolour(189, 49, 115, 255);
    if (pitchInvert == 1) sprintf(currOption, "Invert Pitch: OFF", pitchInvert);
    if (pitchInvert == 2) sprintf(currOption, "Invert Pitch: Half", pitchInvert);
    if (pitchInvert == 3) sprintf(currOption, "Invert Pitch: Full", pitchInvert);
    
    print_small_text_light(16, yPos, currOption, PRINT_ALL, PRINT_ALL, FONT_OUTLINE);
    yPos += 116;

    print_set_envcolour(206, 156, 255, 255);
    sprintf(currOption, "Scroll: %2d", debugScroll);

    print_small_text_light(16, yPos, currOption, PRINT_ALL, PRINT_ALL, FONT_OUTLINE);
}

void debug_stats(void) {
    char debug[32];
    if (gIsConsole) {
        sprintf(debug, "E3 KIOSK", gEmulator); // noway someone's playing my hack on everdrive!!!
    } else {
        (gEmulator < 0x10) ? sprintf(debug, "EMU: 0x000%x", gEmulator) : sprintf(debug, "EMU: 0x00%x", gEmulator);
        sprintf(debug, "EMU: 0x00%x", gEmulator);
    }

    print_small_text_light(16, 210, debug, PRINT_ALL, PRINT_ALL, FONT_OUTLINE);
}

void demo_mode(void) {
     return;
    char textBytes[32];
    sprintf(textBytes, "DEMO", gEmulator);
    print_small_text_light((SCREEN_CENTER_X - 16), 220, textBytes, PRINT_ALL, PRINT_ALL, FONT_OUTLINE);
}

u8 sleepTimer = 0;
u8 sleepInc = 0;

void sleep_draw(void) {
    if (gMarioState->prevAction == ACT_SLEEPING) sleepTimer = 0;
    if (sleepInc > 0) {
        prepare_blank_box();
        render_blank_box(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x10, 0x10, 0x21, (sleepInc * 8 - 1));
        finish_blank_box();
    }
    if (sleepTimer == sleepInc) return;
    if (sleepTimer > sleepInc && gGlobalTimer % 8 == 0) sleepInc++;
    if (sleepTimer < sleepInc) {
        (sleepInc > 0 && (sleepInc - 1) > sleepTimer) 
        ? sleepInc--
        : (sleepInc = sleepTimer); // subtracting would either overflow, or be less than sleepTimer. 
        // unsigned integer skillz 100
    }
}

void z64_draw(void) {
    if (gZ64Toggle) print_small_text_light(16, 28, "LIVES", PRINT_ALL, PRINT_ALL, FONT_OUTLINE);
}

void fps_testing(void) {
    // print_text_fmt_int(160, 32, "%d", (gMarioState->peakHeight - gMarioState->pos[1]) > FALL_DAMAGE_HEIGHT_SMALL);
    /*
    print_text_fmt_int(160, 32, "%d", delta(4, 1, gDeltaTime * 1));
    print_text_fmt_int(160, 16, "%d", deltalite(4, gDeltaTime * 1));
    */

    /*
    char test[32];
    sprintf(test, "%2.1f", (gMarioState->peakHeight - gMarioState->pos[1]));
    print_small_text_light(160, 16, test, PRINT_ALL, PRINT_ALL, FONT_OUTLINE);
    */
}

#include "src/audio/load.h"
#include "src/audio/heap.h"

void testing(void) {
    print_text_fmt_int(160, 64, "%d", gMaxSimultaneousNotes);
    /*
    char debug[64];
    sprintf(debug, "%2.1f", gMarioState->forwardVel);
    print_small_text_light(120, 216, debug, PRINT_ALL, PRINT_ALL, FONT_OUTLINE);
    */
    /*
    struct Surface *floor = gMarioState->floor;
    f32 steepness = sqrtf(floor->normal.x * floor->normal.x + floor->normal.z * floor->normal.z);
    sprintf(debug, "%2.4f", (steepness + 8.0f) * 16.0f);
    print_small_text_light(120, 216, debug, PRINT_ALL, PRINT_ALL, FONT_OUTLINE);
    sprintf(debug, "%2.4f", (steepness / 12.0f) + 1.0f);
    print_small_text_light(120, 224, debug, PRINT_ALL, PRINT_ALL, FONT_OUTLINE);
    */
}

/**
 * Render HUD strings using hudDisplayFlags with it's render functions,
 * excluding the cannon reticle which detects a camera preset for it.
 */
void render_hud(void) {
    fps_calc();
    s16 hudDisplayFlags = gHudDisplay.flags;
    if (hudDisplayFlags == HUD_DISPLAY_NONE) {
        sPowerMeterHUD.animation = POWER_METER_HIDDEN;
        sPowerMeterStoredHealth = 8;
        sPowerMeterVisibleTimer = 0;
#ifdef BREATH_METER
        sBreathMeterHUD.animation = BREATH_METER_HIDDEN;
        sBreathMeterStoredValue = 8;
        sBreathMeterVisibleTimer = 0;
#endif
    } else {
#ifdef VERSION_EU
        // basically create_dl_ortho_matrix but guOrtho screen width is different
        Mtx *mtx = alloc_display_list(sizeof(*mtx));

        if (mtx == NULL) {
            return;
        }

        create_dl_identity_matrix();
        guOrtho(mtx, -16.0f, SCREEN_WIDTH + 16, 0, SCREEN_HEIGHT, -10.0f, 10.0f, 1.0f);
        gSPPerspNormalize(gDisplayListHead++, 0xFFFF);
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(mtx),
                G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
#else
        create_dl_ortho_matrix();
#endif

        handle_stats();
        if (gHudToggle) { mTimer = 1; cTimer = 1; }
#ifdef ENABLE_LIVES
        if (hudDisplayFlags & HUD_DISPLAY_FLAG_LIVES) {
            ms_invis();
            if (mTimerArg < 2) render_hud_mario_lives();
        }
#endif
        if (hudDisplayFlags & HUD_DISPLAY_FLAG_COIN_COUNT) {
            cs_invis();
            if ((!hurtShake || gGlobalTimer % 3 != 0) && cTimerArg < 2) render_hud_coins();
        }

        if (hudDisplayFlags & HUD_DISPLAY_FLAG_STAR_COUNT) {
            if ((!hurtShake || gGlobalTimer % 3 != 0) && cTimerArg < 2) render_hud_stars();
        }

        if (hudDisplayFlags & HUD_DISPLAY_FLAG_KEYS) {
            render_hud_keys();
        }

#ifdef BREATH_METER
        if (hudDisplayFlags & HUD_DISPLAY_FLAG_BREATH_METER) render_hud_breath_meter();
#endif

        if (hudDisplayFlags & HUD_DISPLAY_FLAG_CAMERA_AND_POWER) {
            render_hud_power_meter();
        }

        if (hudDisplayFlags & HUD_DISPLAY_FLAG_TIMER) {
            render_hud_timer();
        }

        attack_timer();
        music_menu();
        if (gDebugToggle) debug_stats();
        sleep_draw();
        // fps_testing();
        // testing();
        // float_test(); /* my testing concludes that sm64 uses ieee 754 floating point :3 
        if (!gHudToggle) return;
        timer_troll();
        if (gDebugLevelSelect && !gLVLToggle) demo_mode();
#ifdef VANILLA_STYLE_CUSTOM_DEBUG
        if (gCustomDebugMode) {
            render_debug_mode();
        }
#endif
    }
}
