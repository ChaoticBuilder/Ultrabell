#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/area.h"
#include "game/level_update.h"
#include "game/smb_engine.h"

#include "levels/scripts.h"

#include "actors/common0.h"
#include "actors/common1.h"

#include "make_const_nonconst.h"
#include "levels/tutorial/header.h"
#include "farcall.h"

#include "config.h"
#include "game/print.h"

#include "game/object_list_processor.h"

const LevelScript level_tutorial_entry[] = {
    INIT_LEVEL(),
    LOAD_LEVEL_DATA(tutorial),
    ALLOC_LEVEL_POOL(),
    AREA(/*index*/ 1, tutorial_geo),
    END_AREA(),
    FREE_LEVEL_POOL(),
    SLEEP(/*frames*/ 1),
    BLACKOUT(/*active*/ FALSE),
    LOAD_AREA(/*area*/ 1),
    SET_MENU_MUSIC(/*seq*/ SEQ_MENU_TITLE_SCREEN),
    CALL_LOOP(/*arg*/ 0, /*func*/ fami_game_loop),
};