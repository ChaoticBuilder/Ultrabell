// sparkle_spawn_star.inc.c

struct ObjectHitbox sSparkleSpawnStarHitbox = {
    /* interactType:      */ INTERACT_STAR_OR_KEY,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 80,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

void bhv_spawned_star_init(void) {
    if (!(o->oInteractionSubtype & INT_SUBTYPE_NO_EXIT)) {
        o->oBehParams = o->parentObj->oBehParams;
    }
    u8 param = GET_BPARAM1(o->oBehParams);
#ifdef GLOBAL_STAR_IDS
    if ((1 << (param % 7)) & save_file_get_star_flags((gCurrSaveFileNum - 1), COURSE_NUM_TO_INDEX(param / 7))) {
#else
    if ((1 << param) & save_file_get_star_flags((gCurrSaveFileNum - 1), COURSE_NUM_TO_INDEX(gCurrCourseNum))) {
#endif
        cur_obj_set_model(MODEL_TRANSPARENT_STAR);
    }

    cur_obj_play_sound_2(SOUND_GENERAL2_STAR_APPEARS);
}

void set_sparkle_spawn_star_hitbox(void) {
    obj_set_hitbox(o, &sSparkleSpawnStarHitbox);
    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        obj_mark_for_deletion(o);
        o->oInteractStatus = INT_STATUS_NONE;
    }
}

void spawned_star_set_target_above_mario(void) {
    vec3f_copy_y_off(&o->oHomeVec, &gMarioObject->oPosVec, 250.0f);

    // Check that the star isn't clipping inside the ceiling
    if (gMarioState->ceil != NULL) {
        if (o->oHomeY > (gMarioState->ceilHeight - 50)) {
            o->oHomeY = gMarioState->ceilHeight - 50;
        }
    }

    o->oPosY = o->oHomeY; 
    f32 lateralDist;
    vec3f_get_lateral_dist(&o->oPosVec, &o->oHomeVec, &lateralDist);

    o->oForwardVel = lateralDist / 23.0f;
}

void set_y_home_to_pos(void) {
    o->oForwardVel = 0.0f;
    o->oHomeY = o->oPosY;
}

void slow_star_rotation(void) {
    if (gGlobalTimer % 2 == 0) o->oAnimState++;
}

void bhv_spawned_star_loop(void) {
    if (o->oAction == SPAWN_STAR_POS_CUTSCENE_ACT_START) {
        if (o->oTimer == 0) {
            cur_obj_become_intangible();
            if (o->oBehParams2ndByte == SPAWN_STAR_POS_CUTSCENE_BP_SPAWN_AT_MARIO) {
                spawned_star_set_target_above_mario();             
            } else {
                set_y_home_to_pos();
            }
            o->oMoveAngleYaw = cur_obj_angle_to_home();
            o->oVelY = 50.0f;
            o->oGravity = -4.0f;
            spawn_mist_particles();
        }
        cur_obj_play_sound_1(SOUND_ENV_STAR);
        spawn_object(o, MODEL_NONE, bhvSparkleSpawn);
        if (o->oVelY < 0 && o->oPosY < o->oHomeY) {
            o->oAction++; // SPAWN_STAR_POS_CUTSCENE_ACT_BOUNCE
            o->oForwardVel = 0;
            o->oVelY = 20.0f;
            o->oGravity = -1.0f;
            play_power_star_jingle();
        }
    } else if (o->oAction == SPAWN_STAR_POS_CUTSCENE_ACT_BOUNCE) {
        if (o->oVelY < -4.0f) {
            o->oVelY = -4.0f;
        }
        if (o->oVelY < 0 && o->oPosY < o->oHomeY) {
            cur_obj_become_tangible();
            gObjCutsceneDone = TRUE;
            o->oVelY = 0;
            o->oGravity = 0;
            o->oAction++; // SPAWN_STAR_POS_CUTSCENE_ACT_END
        }
        spawn_object(o, MODEL_NONE, bhvSparkleSpawn);
    } else if (o->oAction == SPAWN_STAR_POS_CUTSCENE_ACT_END) {
        set_sparkle_spawn_star_hitbox();
        // slow_star_rotation();
    }

    cur_obj_move_using_fvel_and_gravity();
    if (gGlobalTimer % 2 == 0) o->oAnimState++;
    o->oInteractStatus = INT_STATUS_NONE;
}

void bhv_spawn_star_no_level_exit(u32 params) {
    struct Object *starObj = spawn_object(o, MODEL_STAR, bhvSpawnedStarNoLevelExit);
    SET_BPARAM1(starObj->oBehParams, params);
    starObj->oInteractionSubtype = INT_SUBTYPE_NO_EXIT;
    obj_set_angle(starObj, 0x0, 0x0, 0x0);
}
