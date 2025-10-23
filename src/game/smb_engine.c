#include <PR/ultratypes.h>

#include "audio/external.h"
#include "engine/math_util.h"
#include "area.h"
#include "game_init.h"
#include "level_update.h"
#include "main.h"
#include "memory.h"
#include "print.h"
#include "save_file.h"
#include "sound_init.h"
#include "level_table.h"
#include "seq_ids.h"
#include "sm64.h"
#include "smb_engine.h"
#include "ingame_menu.h"

void init_famicom_mario(void) {
    gFamiState->input = 0;
    gFamiState->actionTimer = 0;
    gFamiState->invincTimer = 0;
    gFamiState->powerTimer = 0;
    gFamiState->actionState = 0;
    gFamiState->action = 0; // IDLE
    gFamiState->prevAction = 0;
}

void init_famicom_mario_titlescr(void) {
    init_famicom_mario();
    gFamiState->powerState = 0; // Small Mario
    gFamiState->health = 2;
    gFamiState->numLives = 2;
    gFamiState->numCoins = 0;
}

u8 fami_game_loop(void) {
    print_intro_text();
    return TRUE;
}