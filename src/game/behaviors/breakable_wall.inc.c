// breakable_wall.inc.c

void bhv_wf_breakable_wall_loop(void) {
    if (gMarioState->action == ACT_SHOT_FROM_CANNON || gMarioState->action == ACT_FLYING) {
        cur_obj_become_tangible();

        if (obj_check_if_collided_with_object(o, gMarioObject)) {
            if (cur_obj_has_behavior(bhvWfBreakableWallRight)) {
                play_puzzle_jingle();
            }

            create_sound_spawner(SOUND_GENERAL_WALL_EXPLOSION);

            o->oInteractType = INTERACT_DAMAGE;
            o->oDamageOrCoinValue = 1;

            obj_explode_and_spawn_coins(80.0f, COIN_TYPE_NONE);
        }
    } else {
        cur_obj_become_intangible();
    }
}
