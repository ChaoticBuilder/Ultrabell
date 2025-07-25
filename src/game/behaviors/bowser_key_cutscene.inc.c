// bowser_key_cutscene.inc.c

Gfx *geo_scale_bowser_key(s32 callContext, struct GraphNode *node, UNUSED Mat4 mtx) {
    if (callContext == GEO_CONTEXT_RENDER) {
        struct Object *nodeObj = (struct Object *) gCurGraphNodeObject;
        ((struct GraphNodeScale *) node->next)->scale = nodeObj->oBowserKeyScale;
    }
    return NULL;
}

void bhv_bowser_key_unlock_door_loop(void) {
    s32 animFrame = o->header.gfx.animInfo.animFrame;

    cur_obj_init_animation_with_sound(0);

    if (animFrame < 38 * gDeltaTime) {
        o->oBowserKeyScale = 0.0f;
    } else if (animFrame < 49 * gDeltaTime) {
        o->oBowserKeyScale = 0.2f;
    } else if (animFrame < 58 * gDeltaTime) {
        o->oBowserKeyScale = (animFrame - (53 * gDeltaTime)) * 0.11875f + 0.2f; // 0.11875f?
    } else if (animFrame < 59 * gDeltaTime) {
        o->oBowserKeyScale = 1.1f;
    } else if (animFrame < 60 * gDeltaTime) {
        o->oBowserKeyScale = 1.05f;
    } else {
        o->oBowserKeyScale = 1.0f;
    }

    if (o->oTimer > 150 * gDeltaTime) {
        obj_mark_for_deletion(o);
    }
}

void bhv_bowser_key_course_exit_loop(void) {
    s32 animFrame = o->header.gfx.animInfo.animFrame;

    cur_obj_init_animation_with_sound(1);

    if (animFrame < 38 * gDeltaTime) {
        o->oBowserKeyScale = 0.2f;
    } else if (animFrame < 52 * gDeltaTime) {
        o->oBowserKeyScale = (animFrame - (42 * gDeltaTime)) * (3.0f / 70.0f) + 0.2f;
    } else if (animFrame < 94 * gDeltaTime) {
        o->oBowserKeyScale = 0.8f;
    } else if (animFrame < 101 * gDeltaTime) {
        o->oBowserKeyScale = ((101 * gDeltaTime) - animFrame) * (6.0f / 70.0f) + 0.2f;
    } else {
        o->oBowserKeyScale = 0.2f;
    }

    if (o->oTimer > 138 * gDeltaTime) {
        obj_mark_for_deletion(o);
    }
}
