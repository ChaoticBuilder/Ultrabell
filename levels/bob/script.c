#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/level_update.h"

#include "levels/scripts.h"

#include "actors/common1.h"
#include "make_const_nonconst.h"
#include "levels/bob/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_bob_entry[] = {
	INIT_LEVEL(),
	LOAD_YAY0(0x07, _bob_segment_7SegmentRomStart, _bob_segment_7SegmentRomEnd), 
	LOAD_YAY0_TEXTURE(0x09, _generic_yay0SegmentRomStart, _generic_yay0SegmentRomEnd), 
	LOAD_YAY0(0x0A, _water_skybox_yay0SegmentRomStart, _water_skybox_yay0SegmentRomEnd), 
	LOAD_YAY0(0x05, _group3_yay0SegmentRomStart, _group3_yay0SegmentRomEnd), 
	LOAD_RAW(0x0C, _group3_geoSegmentRomStart, _group3_geoSegmentRomEnd), 
	LOAD_YAY0(0x06, _group14_yay0SegmentRomStart, _group14_yay0SegmentRomEnd), 
	LOAD_RAW(0x0D, _group14_geoSegmentRomStart, _group14_geoSegmentRomEnd), 
	LOAD_YAY0(0x08, _common0_yay0SegmentRomStart, _common0_yay0SegmentRomEnd), 
	LOAD_RAW(0x0F, _common0_geoSegmentRomStart, _common0_geoSegmentRomEnd), 
	LOAD_YAY0(0xb, _effect_yay0SegmentRomStart, _effect_yay0SegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_1), 
	JUMP_LINK(script_func_global_4), 
	JUMP_LINK(script_func_global_15), 
	LOAD_MODEL_FROM_GEO(MODEL_BOB_BUBBLY_TREE, bubbly_tree_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_BOB_CHAIN_CHOMP_GATE, bob_geo_000440), 
	LOAD_MODEL_FROM_GEO(MODEL_BOB_SEESAW_PLATFORM, bob_geo_000458), 
	LOAD_MODEL_FROM_GEO(MODEL_BOB_BARS_GRILLS, bob_geo_000470), 
	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, bob_area_1),
		WARP_NODE(0xF0, LEVEL_CASTLE, 0x01, 0x32, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_CASTLE, 0x01, 0x64, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0B, LEVEL_BOB, 0x01, 0x0B, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0C, LEVEL_BOB, 0x01, 0x0C, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_BOBOMB_BUDDY, -4560, 0, 5120, 0, 0, 0, DIALOG_004 << 16, bhvBobombBuddy),
		OBJECT(MODEL_BREAKABLE_BOX, -4100, 0, 5020, 0, 0, 0, DIALOG_004 << 16, bhvBreakableBox),
		OBJECT(MODEL_BREAKABLE_BOX, -3500, 40, 4980, 0, 0, 0, DIALOG_004 << 16, bhvBreakableBox),
		OBJECT(MODEL_BREAKABLE_BOX, -3500, 40, 5180, 0, 0, 0, DIALOG_004 << 16, bhvBreakableBox),
		OBJECT(MODEL_CHECKERBOARD_PLATFORM, -2303, 717, 1024, 0, 45, 0, 0x00140000, bhvCheckerboardElevatorGroup),
		OBJECT(MODEL_CHAIN_CHOMP, 770, 768, 1600, 0, 0, 0, 0x00000000, bhvChainChomp),
		OBJECT(MODEL_NONE, -6000, 1270, -5600, 0, 0, 0, 0x02000000, bhvHiddenRedCoinStar),
		OBJECT_WITH_ACTS(MODEL_NONE, 3072, 4194, -4800, 0, 0, 0, 0x00000000, bhvKoopaRaceEndpoint, ACT_2 | ACT_3 | ACT_4 | ACT_5 | ACT_6),
		OBJECT_WITH_ACTS(MODEL_KOOPA_FLAG, 3072, 4194, -4800, 0, 0, 0, 0x00000000, bhvKoopaFlag, ACT_2 | ACT_3 | ACT_4 | ACT_5 | ACT_6),
		MARIO_POS(0x01, 135, -6558, 100, 6464),
		OBJECT(MODEL_STAR, -6900, 320, -6000, 0, 0, 0, 0x03000000, bhvStar),
		OBJECT(MODEL_STAR, 1636, 4450, -5567, 0, 0, 0, 0x00000000, bhvStar),
		OBJECT(MODEL_STAR, -1536, 1536, -5600, 0, 0, 0, 0x04000000, bhvStar),
		OBJECT(MODEL_NONE, -6558, 1000, 6464, 0, 135, 0, 0x000A0000, bhvSpinAirborneWarp),
		TERRAIN(bob_area_1_collision),
		MACRO_OBJECTS(bob_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_LEVEL_GRASS),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	FREE_LEVEL_POOL(),
	MARIO_POS(0x01, 135, -6558, 100, 6464),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};