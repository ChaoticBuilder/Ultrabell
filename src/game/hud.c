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

#include "config.h"

s32 gTimeAttackToggle = FALSE;
u8 gLuigiToggle = 0;
s32 gSecondsToggle = TRUE;
u8 gHudToggle;
u8 troll = FALSE;

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

void print_fps(s32 x, s32 y) {
#ifdef USE_PROFILER
    f32 fps = profiler_get_fps();
#else
    f32 fps = calculate_and_update_fps();
#endif
    char text[14];

    sprintf(text, "FPS %2.2f", fps);
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

struct CameraHUD {
    s16 status;
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
s32 sPowerMeterVisibleTimer = 0;

#ifdef BREATH_METER
static s16 sBreathMeterStoredValue;
static struct PowerMeterHUD sBreathMeterHUD = {
    BREATH_METER_HIDDEN,
    HUD_BREATH_METER_X,
    HUD_BREATH_METER_HIDDEN_Y,
};
s32 sBreathMeterVisibleTimer = 0;
#endif

//static struct CameraHUD sCameraHUD = { CAM_STATUS_NONE };

s32 gHudShakeX;
s32 gHudShakeY;

void hud_shake(void) {
    /* DISABLED FEATURE:
    print_text_fmt_int(268, 178, "%d", gHudShakeX);
    print_text_fmt_int(268, 161, "%d", gHudShakeY);
    print_text(136 + gHudShakeX, 112 + gHudShakeY, "TEST");
    */
}

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

    guTranslate(mtx, (f32) sPowerMeterHUD.x + gHudShakeX, (f32) sPowerMeterHUD.y + gHudShakeY, 0);

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
    s16 hudDisplayFlags = gHudDisplay.flags;

    if (!(hudDisplayFlags & HUD_DISPLAY_FLAG_EMPHASIZE_POWER)) {
        if (sPowerMeterVisibleTimer == 30.0f) {
            sPowerMeterHUD.animation = POWER_METER_DEEMPHASIZING;
        }
    } else {
        sPowerMeterVisibleTimer = 0;
    }
}

/**
 * Power meter animation called after emphasized mode.
 * Moves power meter y pos speed until it's at 200 to be visible.
 */
static void animate_power_meter_deemphasizing(void) {
    s16 speed = 5;

    if (sPowerMeterHUD.y > HUD_POWER_METER_Y - 20) speed = 3;
    if (sPowerMeterHUD.y > HUD_POWER_METER_Y - 10) speed = 2;
    if (sPowerMeterHUD.y > HUD_POWER_METER_Y -  5) speed = 1;

    sPowerMeterHUD.y += speed;

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
    sPowerMeterHUD.y += 20;
    if (sPowerMeterHUD.y > HUD_POWER_METER_HIDDEN_Y) {
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
        sPowerMeterHUD.y = HUD_POWER_METER_EMPHASIZED_Y;
    }

    // Show power meter if health is full, has 8
    if (numHealthWedges == 8 && sPowerMeterStoredHealth == 7) {
        sPowerMeterVisibleTimer = 0;
    }

    // After health is full, hide power meter
    if (numHealthWedges == 8 && sPowerMeterVisibleTimer == 30.0f) {
        sPowerMeterHUD.animation = POWER_METER_HIDING;
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
    sPowerMeterVisibleTimer++;
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


/**
 * Renders the amount of lives Mario has.
 */
s32 hurtShake = FALSE;

void render_hud_mario_lives(void) {
    if (gMarioState->hurtCounter != 0) {
        hurtShake = TRUE;
    }
    if ((gHudShakeX || gHudShakeY) != 0 && hurtShake == TRUE) {
        print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(16) + gHudShakeX, HUD_TOP_Y + gHudShakeY, "ROLA!");
    } else {
        hurtShake = FALSE;
        if (!gLuigiToggle) {
            print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(16) + gHudShakeX, HUD_TOP_Y + gHudShakeY, ","); // 'Mario' glyph
        } else {
            print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(16) + gHudShakeX, HUD_TOP_Y + gHudShakeY, ";"); // 'Luigi' glyph
        }

        print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(31) + gHudShakeX, HUD_TOP_Y + gHudShakeY, "*"); // 'X' glyph
        print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(46) + gHudShakeX, HUD_TOP_Y + gHudShakeY, "%d", gHudDisplay.lives);
    }
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
    if (gHudDisplay.coins == 1996) {
        // Thank you-a so much for-a playing my game!
        print_text_fmt_int((GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(HUD_STARS_X)) + gHudShakeX, (HUD_TOP_Y - 17) + gHudShakeY, "%d", gHudDisplay.coins);
    } else {
        print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(HUD_STARS_X) + gHudShakeX, (HUD_TOP_Y - 17) + gHudShakeY, "$"); // 'Coin' glyph
        print_text((GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(HUD_STARS_X - 16)) + gHudShakeX, (HUD_TOP_Y - 17) + gHudShakeY, "*"); // 'X' glyph
        print_text_fmt_int((GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(HUD_STARS_X - 31)) + gHudShakeX, (HUD_TOP_Y - 17) + gHudShakeY, "%d", gHudDisplay.coins);
    }
}

/**
 * Renders the amount of stars collected.
 * Disables "X" glyph when Mario has 100 stars or more.
 */
void render_hud_stars(void) {
    // if (gHudFlash == HUD_FLASH_STARS && gGlobalTimer & 0x8) return;
    print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(HUD_STARS_X) + gHudShakeX, HUD_TOP_Y + gHudShakeY, "^"); // 'Star' glyph
    print_text((GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(HUD_STARS_X - 16)) + gHudShakeX, HUD_TOP_Y + gHudShakeY, "*"); // 'X' glyph
    print_text_fmt_int((GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(HUD_STARS_X - 31)) + gHudShakeX, HUD_TOP_Y + gHudShakeY, "%d", gHudDisplay.stars);
}

/**
 * Unused function that renders the amount of keys collected.
 * Leftover function from the beta version of the game.
 */
void render_hud_keys(void) {
    s16 i;

    for (i = 0; i < gHudDisplay.keys; i++) {
        print_text((i * 16) + 220, 142, "|"); // unused glyph - beta key
    }
}

/**
 * Renders the timer when Mario start sliding in PSS.
 */
void render_hud_timer(void) {
    Texture *(*hudLUT)[58] = segmented_to_virtual(&main_hud_lut);
    u16 timerValFrames = gHudDisplay.timer;
    u16 timerMins = timerValFrames / (30 * 60);
    u16 timerSecs = (timerValFrames - (timerMins * 1800)) / 30;
    u16 timerFracSecs = ((timerValFrames - (timerMins * 1800) - (timerSecs * 30)) & 0xFFFF) / 3;

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

#ifdef DEMO_TIMER
void render_hud_demo_timer(void) {
    char clockBytes[12];
    int timerCount = gGlobalTimer / 30;
    int timerCount2 = timerCount / 60;
    int timerCount3 = timerCount2 / 60;
    int timerSeconds = timerCount % 60;
    int timerMinutes = timerCount2 % 60;
    int timerHours = timerCount3 % 99; // just incase if the globaltimer even lasts for this long tbh
    sprintf(clockBytes, "Clock: %02d : %02d . %02d", timerHours, timerMinutes, timerSeconds);
    #ifdef TIME_ATTACK
        int timeATKCount = 0;
        if (gGlobalTimer % 30) timeATKCount++;
        int timeATKCount2 = timeATKCount / 60;
        int timeATKSecs = timeATKCount % 60;
        int timeATKMins = timeATKCount2 % 60;
        if (gTimeAttackToggle == TRUE) {
            if (!(COURSE_IS_MAIN_COURSE(gCurrCourseNum))) return;
            print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(128), HUD_BOTTOM_Y, "SPEEDRUN");
            if (timeAttackMinutes > TIME_ATTACK) {
                gMarioState->hurtCounter = 31;
                gGlobalTimer = 0;
            }
            print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(16), HUD_BOTTOM_Y, "TIME");
            print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(68), HUD_BOTTOM_Y, "%02d", (timeAttackMinutes % 60));
            print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(102), HUD_BOTTOM_Y, "%02d", (timeAttackSeconds % 60));
            print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(136), HUD_BOTTOM_Y, "%02d", (timeAttackCount % 30));
        
            gSPDisplayList(gDisplayListHead++, dl_hud_img_begin);
            render_hud_tex_lut(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(91), (HUD_TOP_Y + 1), (*hudLUT)[GLYPH_APOSTROPHE]);
            render_hud_tex_lut(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(91), (HUD_TOP_Y - 8), (*hudLUT)[GLYPH_APOSTROPHE]);
            render_hud_tex_lut(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(125), (HUD_TOP_Y + 1), (*hudLUT)[GLYPH_APOSTROPHE]);
        }
    #endif
    // WIP, I figured out externs can be global, so this may happen soon
    
    if (gTimeAttackToggle) return;
    print_set_envcolour(0, 189, 255, 255);
    print_small_text_light(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(16) + gHudShakeX, (HUD_TOP_Y + 9) + gHudShakeY, clockBytes, PRINT_ALL, PRINT_ALL, FONT_OUTLINE);
}

void timer_troll(void) {
    int trollCount = (gGlobalTimer / 15) % 1800;
    int trollCount2 = gGlobalTimer % 27000;
    if (!gTrollToggle) {
        f32 rand = random_float();
        /*
        print_text_fmt_int(160, 48, "%2d", troll);
        print_text_fmt_int(160, 32, "%2d", trollCount);
        print_text_fmt_int(160, 16, "%2d", trollCount2);
        */
        if (trollCount2 == 0 && rand <= 0.5f) {
            troll = TRUE;
        }
        if (troll == TRUE) {
            if (gGlobalTimer % 2 != 0) return;
            if (rand < 0.125f) {
                if (trollCount == 0) return;
                troll = FALSE;
            }
            if (rand < 0.25f) return print_text(96, 120, "TURN IT OFF");
            if (rand < 0.375f) return print_text(140, 120, "HELP");
            if (rand < 0.5f) return print_text(140, 120, "STOP");
            if (rand < 0.625f) return print_text(24, 120, "43N9F6DT4ZUOFXQDFQJ0K8VZ");
            if (rand < 0.75f) return print_text(24, 120, "XW12A55O1I3T8VIZKV1Z6N5M");
            if (rand < 0.875f) return print_text(24, 120, "23HA79CUOSSI0PPXC8CAS46E");
            return print_text(24, 120, "9C0AIY5H45I37L5WIR9D61G1");
            // imagine if I didn't if guard this shit it'd be nightmare spaghetti code
            // why did I even code this what possessed me to do this
        }
    }
}
#endif

// welcome to disabled feature city
// the residents here welcome your stay

/* DISABLED FEATURE: (for now until 2013!!1!1!)
void unregistered_hypercam(void) {
    print_text(32, 80, "UNREGISTERED HYPERCAM");
}
*/

/* DISABLED FEATURE:
void ttc(void) {
    print_text_fmt_int(120, 16, "state %d", gTTCSpeedSetting);
}
*/

/**
 * Sets HUD status camera value depending of the actions
 * defined in update_camera_status.
 */
/* DISABLED FEATURE:
void set_hud_camera_status(s16 status) {
    sCameraHUD.status = status;
}
*/

/**
 * Renders camera HUD glyphs using a table list, depending of
 * the camera status called, a defined glyph is rendered.
 */
/* DISABLED FEATURE:
void render_hud_camera_status(void) {
    Texture *(*cameraLUT)[6] = segmented_to_virtual(&main_hud_camera_lut);
    s32 x = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(HUD_CAMERA_X);
    s32 y = 205;

    if (sCameraHUD.status == CAM_STATUS_NONE) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, dl_hud_img_begin);
    render_hud_tex_lut(x, y, (*cameraLUT)[GLYPH_CAM_CAMERA]);

    switch (sCameraHUD.status & CAM_STATUS_MODE_GROUP) {
        case CAM_STATUS_MARIO:
            render_hud_tex_lut(x + 16, y, (*cameraLUT)[GLYPH_CAM_MARIO_HEAD]);
            break;
        case CAM_STATUS_LAKITU:
            render_hud_tex_lut(x + 16, y, (*cameraLUT)[GLYPH_CAM_LAKITU_HEAD]);
            break;
        case CAM_STATUS_FIXED:
            render_hud_tex_lut(x + 16, y, (*cameraLUT)[GLYPH_CAM_FIXED]);
            break;
    }

    switch (sCameraHUD.status & CAM_STATUS_C_MODE_GROUP) {
        case CAM_STATUS_C_DOWN:
            render_hud_small_tex_lut(x + 4, y + 16, (*cameraLUT)[GLYPH_CAM_ARROW_DOWN]);
            break;
        case CAM_STATUS_C_UP:
            render_hud_small_tex_lut(x + 4, y - 8, (*cameraLUT)[GLYPH_CAM_ARROW_UP]);
            break;
    }

    gSPDisplayList(gDisplayListHead++, dl_hud_img_end);
}
*/

/**
 * Render HUD strings using hudDisplayFlags with it's render functions,
 * excluding the cannon reticle which detects a camera preset for it.
 */
void render_hud(void) {
    s16 hudDisplayFlags = gHudDisplay.flags;
    if (!gHudToggle) {
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

#ifdef ENABLE_LIVES
            if (hudDisplayFlags & HUD_DISPLAY_FLAG_LIVES) {
                render_hud_mario_lives();
            }
#endif

            if (hudDisplayFlags & HUD_DISPLAY_FLAG_COIN_COUNT) {
                render_hud_coins();
            }

            if (hudDisplayFlags & HUD_DISPLAY_FLAG_STAR_COUNT) {
                render_hud_stars();
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
                render_hud_demo_timer();
                timer_troll();
                visualizer_display();
                // ttc(); displays the clock's current state, technically I could've kept it in but nah
                // what makes me mad tho is the fact I had to do this in the first place
                // these dumbass programmers coded the clock super weirdly so I had to figure out how to make it normal
                hud_shake();

#ifdef VANILLA_STYLE_CUSTOM_DEBUG
            if (gCustomDebugMode) {
                render_debug_mode();
            }
#endif
        }
    }
}
