/* clang-format off */
/*
 * mem_error_screen.inc.c
 *
 * This enhancement should be used for ROM hacks that require the expansion pak.
 *
 */
/* clang-format on */

#include <types.h>
#include "segments.h"
#include "text_strings.h"
#include "game_init.h"
#include "main.h"
#include "print.h"
#include "ingame_menu.h"
#include "segment2.h"
#include "../engine/level_script.h"
#include "audio/external.h"

#include "sm64.h"

// Require 8 MB of RAM, even if the pool doesn't go into extended memory.
// Change the '8' to whatever MB limit you want.
// Note: only special emulators allow for RAM sizes above 8 MB.
#define REQUIRED_MB (RAM_1MB * 8)

u8 gNotEnoughMemory = FALSE;
u8 gNoEXTRAM = FALSE;
u8 gDelayForErrorMessage = 0;

// Ensure that USE_EXT_RAM is defined.
#ifndef USE_EXT_RAM
gNotEnoughMemory = TRUE;
gNoEXTRAM = TRUE;
#endif

u8 does_pool_end_lie_out_of_bounds(void *end) {
    u32 endPhy = ((u32) end) & 0x1FFFFFFF;
    u32 memSize = *((u32 *) 0x80000318);

    if (endPhy > memSize) {
        gNotEnoughMemory = TRUE;
        return TRUE;
    } else {
        if (memSize < REQUIRED_MB) {
            gNotEnoughMemory = TRUE;
        }
        return FALSE;
    }
}

// An Expansion Pak is required to play this ROM Hack!
u8 textExpak[] = { EXPAK_REQUIRED };
u8 textExtram[] = { EXT_RAM };

Gfx *geo18_display_error_message(u32 run) {
    if (run) {
        if (gDelayForErrorMessage > 0) {
            // Draw color text title.
            print_text_centered(160, 180, "ERROR");

            // Init generic text rendering
            create_dl_ortho_matrix();
            gSPDisplayList(gDisplayListHead++,
                           dl_ia_text_begin); // Init rendering stuff for generic text

            // Set text color to white
            gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
            if (gNoEXTRAM) print_generic_string((SCREEN_CENTER_X - 80), (SCREEN_CENTER_Y + 24), textExtram);
            else           print_generic_string((SCREEN_CENTER_X - 80), (SCREEN_CENTER_Y + 24), textExpak);

            // Cleanup
            gSPDisplayList(gDisplayListHead++,
                           dl_ia_text_end); // Reset back to default render settings.
            gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
        } else {
            gDelayForErrorMessage += 1;
        }
    }

    return FALSE;
}

// Basic main loop for the error screen. Note that controllers are not enabled here.
void thread5_mem_error_message_loop(UNUSED void *arg) {
    struct LevelCommand *addr;

    setup_game_memory();
    set_vblank_handler(2, &gGameVblankHandler, &gGameVblankQueue, (OSMesg) 1);

    addr = segmented_to_virtual(level_script_entry_error_screen);

    render_init();

    while (TRUE) {
        select_gfx_pool();
        addr = level_script_execute(addr);
        display_and_vsync();
    }
}