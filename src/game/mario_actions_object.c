#include <PR/ultratypes.h>

#include "sm64.h"
#include "mario_actions_object.h"
#include "types.h"
#include "mario_step.h"
#include "mario.h"
#include "audio/external.h"
#include "interaction.h"
#include "engine/math_util.h"
#include "rumble_init.h"
#include "ingame_menu.h"
#include "dialog_ids.h"

void animated_stationary_ground_step(struct MarioState *m, s32 animation, u32 endAction) {
    stationary_ground_step(m);
    set_mario_animation(m, animation);
    if (is_anim_at_end(m)) {
        set_mario_action(m, endAction, 0);
    }
}

s32 mario_update_punch_sequence(struct MarioState *m) {
    u32 endAction, crouchEndAction;
    s32 animFrame;

    if (m->action & ACT_FLAG_MOVING) {
        endAction = ACT_WALKING, crouchEndAction = ACT_CROUCH_SLIDE;
    } else {
        endAction = ACT_IDLE, crouchEndAction = ACT_CROUCHING;
    }

    switch (m->actionArg) {
        case ACT_ARG_SOUND1:
            play_sound(SOUND_ACTION_THROW, m->marioObj->header.gfx.cameraToObject);
            FALL_THROUGH;
        case ACT_ARG_PUNCH_SEQUENCE_FIRST_PUNCH:
            set_mario_animation(m, MARIO_ANIM_FIRST_PUNCH);
            if (is_anim_past_end(m)) {
                m->actionArg = ACT_ARG_PUNCH_SEQUENCE_FIRST_PUNCH_FAST;
            } else {
                m->actionArg = ACT_ARG_PUNCH_SEQUENCE_FIRST_PUNCH;
            }

            if (m->marioObj->header.gfx.animInfo.animFrame >= 2) {
                if (mario_check_object_grab(m)) {
                    return TRUE;
                }

                m->flags |= MARIO_PUNCHING;
            }

            if (m->actionArg == ACT_ARG_PUNCH_SEQUENCE_FIRST_PUNCH_FAST) {
                m->marioBodyState->punchState = (PUNCH_STATE_TYPE_FIRST_PUNCH | 0x4);
            }
            break;

        case ACT_ARG_PUNCH_SEQUENCE_FIRST_PUNCH_FAST:
            set_mario_animation(m, MARIO_ANIM_FIRST_PUNCH_FAST);

            if (m->marioObj->header.gfx.animInfo.animFrame <= 0) {
                m->flags |= MARIO_PUNCHING;
            }

            if (m->input & INPUT_B_PRESSED) {
                m->actionArg = ACT_ARG_SOUND2;
            }

            if (is_anim_at_end(m)) {
                set_mario_action(m, endAction, 0);
            }
            break;

        case ACT_ARG_SOUND2:
            play_sound(SOUND_ACTION_SPIN, m->marioObj->header.gfx.cameraToObject);
            FALL_THROUGH;
        case ACT_ARG_PUNCH_SEQUENCE_GROUND_KICK:
            animFrame = set_mario_animation(m, MARIO_ANIM_GROUND_KICK);
            if (animFrame == 0) {
                m->marioBodyState->punchState = (PUNCH_STATE_TYPE_KICK | 0x6);
            }

            if (animFrame >= 0 && animFrame < 8) {
                m->flags |= MARIO_KICKING;
            }

            if (is_anim_at_end(m)) {
                set_mario_action(m, endAction, 0);
            } else {
                m->actionArg = ACT_ARG_PUNCH_SEQUENCE_GROUND_KICK;
            }
            break;

        case ACT_ARG_PUNCH_SEQUENCE_BREAKDANCE:
            create_dialog_box(DIALOG_038);
            if (g95Toggle) {
                return set_mario_action(m, ACT_CROUCHING, 0);
            }
            play_mario_action_sound(m, SOUND_MARIO_PUNCH_HOO, 1);
            set_mario_animation(m, MARIO_ANIM_BREAKDANCE);
            animFrame = m->marioObj->header.gfx.animInfo.animFrame;

            if (animFrame >= 2 && animFrame < 8) {
                m->flags |= MARIO_TRIPPING;
            }

            if (is_anim_at_end(m)) {
                set_mario_action(m, crouchEndAction, 0);
            }
            break;
    }

    return FALSE;
}

s32 act_punching(struct MarioState *m) {
    if (m->input & INPUT_STOMPED) {
        return drop_and_set_mario_action(m, ACT_SHOCKWAVE_BOUNCE, 0);
    }

    if (m->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE)) {
        return check_common_action_exits(m);
    }

    if (m->actionState == ACT_STATE_PUNCHING_CAN_JUMP_KICK && m->input & INPUT_A_DOWN) {
        if (gDiveToggle != 1) return set_mario_action(m, ACT_JUMP_KICK, 0);
        return set_mario_action(m, ACT_JUMP, 0);
    }

    m->actionState = ACT_STATE_PUNCHING_NO_JUMP_KICK;
    if (m->actionArg == 0) {
        m->actionTimer = 7;
    }

    if (m->actionTimer > 0) {
        m->actionTimer--;
    }

    mario_update_punch_sequence(m);
    perform_ground_step(m);
    return FALSE;
}

s32 act_picking_up(struct MarioState *m) {
    if (m->input & INPUT_STOMPED) {
        return drop_and_set_mario_action(m, ACT_SHOCKWAVE_BOUNCE, 0);
    }

    if (m->input & INPUT_OFF_FLOOR) {
        return drop_and_set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (m->actionState == ACT_STATE_PICKING_UP_GRAB && is_anim_at_end(m)) {
        //! While the animation is playing, it is possible for the used object
        // to unload. This allows you to pick up a vacant or newly loaded object
        // slot (cloning via fake object).
        mario_grab_used_object(m);
        play_sound_if_no_flag(m, SOUND_MARIO_HRMM, MARIO_MARIO_SOUND_PLAYED);
        m->actionState = ACT_STATE_PICKING_UP_HOLDING;
    }

    if (m->actionState == ACT_STATE_PICKING_UP_HOLDING) {
        if (m->heldObj->oInteractionSubtype & INT_SUBTYPE_GRABS_MARIO) {
            m->marioBodyState->grabPos = GRAB_POS_HEAVY_OBJ;
            set_mario_animation(m, MARIO_ANIM_GRAB_HEAVY_OBJECT);
            if (is_anim_at_end(m)) {
                set_mario_action(m, ACT_HOLD_HEAVY_IDLE, 0);
            }
        } else {
            m->marioBodyState->grabPos = GRAB_POS_LIGHT_OBJ;
            set_mario_animation(m, MARIO_ANIM_PICK_UP_LIGHT_OBJ);
            if (is_anim_at_end(m)) {
                set_mario_action(m, ACT_HOLD_IDLE, 0);
            }
        }
    }

    stationary_ground_step(m);
    return FALSE;
}

s32 act_dive_picking_up(struct MarioState *m) {
    if (m->input & INPUT_STOMPED) {
        return drop_and_set_mario_action(m, ACT_SHOCKWAVE_BOUNCE, 0);
    }

#ifdef HANDS_FREE_HOLDING_FIX
    if (m->input & INPUT_OFF_FLOOR) {
        return drop_and_set_mario_action(m, ACT_FREEFALL, 0);
    }
    if (m->input & INPUT_ABOVE_SLIDE) {
        return drop_and_set_mario_action(m, ACT_BEGIN_SLIDING, 0);
    }
#else
    //! Hands-free holding. Landing on a slope or being pushed off a ledge while
    // landing from a dive grab sets Mario's action to a non-holding action
    // without dropping the object, causing the hands-free holding glitch.
    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (m->input & INPUT_ABOVE_SLIDE) {
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);
    }
#endif

    animated_stationary_ground_step(m, MARIO_ANIM_STOP_SLIDE_LIGHT_OBJ, ACT_HOLD_IDLE);
    return FALSE;
}

s32 act_placing_down(struct MarioState *m) {
    if (m->input & INPUT_STOMPED) {
        return drop_and_set_mario_action(m, ACT_SHOCKWAVE_BOUNCE, 0);
    }

    if (m->input & INPUT_OFF_FLOOR) {
        return drop_and_set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (++m->actionTimer == 8) {
        mario_drop_held_object(m);
    }

    animated_stationary_ground_step(m, MARIO_ANIM_PLACE_LIGHT_OBJ, ACT_IDLE);
    return FALSE;
}

s32 act_throwing(struct MarioState *m) {
    if (m->heldObj && (m->heldObj->oInteractionSubtype & INT_SUBTYPE_HOLDABLE_NPC)) {
        return set_mario_action(m, ACT_PLACING_DOWN, 0);
    }

    if (m->input & INPUT_STOMPED) {
        return drop_and_set_mario_action(m, ACT_SHOCKWAVE_BOUNCE, 0);
    }

    if (m->input & INPUT_OFF_FLOOR) {
        return drop_and_set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (++m->actionTimer == 7) {
        mario_throw_held_object(m);
        play_sound_if_no_flag(m, SOUND_MARIO_WAH2, MARIO_MARIO_SOUND_PLAYED);
        play_sound_if_no_flag(m, SOUND_ACTION_THROW, MARIO_ACTION_SOUND_PLAYED);
#if ENABLE_RUMBLE
        queue_rumble_data(3, 50);
#endif
    }

    animated_stationary_ground_step(m, MARIO_ANIM_GROUND_THROW, ACT_IDLE);
    return FALSE;
}

s32 act_heavy_throw(struct MarioState *m) {
    if (m->input & INPUT_STOMPED) {
        return drop_and_set_mario_action(m, ACT_SHOCKWAVE_BOUNCE, 0);
    }

    if (m->input & INPUT_OFF_FLOOR) {
        return drop_and_set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (++m->actionTimer == 13) {
        mario_drop_held_object(m);
        play_sound_if_no_flag(m, SOUND_MARIO_WAH2, MARIO_MARIO_SOUND_PLAYED);
        play_sound_if_no_flag(m, SOUND_ACTION_THROW, MARIO_ACTION_SOUND_PLAYED);
#if ENABLE_RUMBLE
        queue_rumble_data(3, 50);
#endif
    }

    animated_stationary_ground_step(m, MARIO_ANIM_HEAVY_THROW, ACT_IDLE);
    return FALSE;
}

s32 act_stomach_slide_stop(struct MarioState *m) {
    if (m->input & INPUT_STOMPED) {
        return set_mario_action(m, ACT_SHOCKWAVE_BOUNCE, 0);
    }

    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (m->input & INPUT_ABOVE_SLIDE) {
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);
    }

    animated_stationary_ground_step(m, MARIO_ANIM_SLOW_LAND_FROM_DIVE, ACT_IDLE);
    return FALSE;
}

s32 act_picking_up_bowser(struct MarioState *m) {
    if (m->actionState == ACT_STATE_PICKING_UP_GRAB) {
        m->actionState = ACT_STATE_PICKING_UP_HOLDING;
        m->angleVel[1] = 0;
        m->marioBodyState->grabPos = GRAB_POS_BOWSER;
        mario_grab_used_object(m);
#if ENABLE_RUMBLE
        queue_rumble_data(5, 80);
#endif
        play_sound(SOUND_MARIO_HRMM, m->marioObj->header.gfx.cameraToObject);
    }

    set_mario_animation(m, MARIO_ANIM_GRAB_BOWSER);
    if (is_anim_at_end(m)) {
        set_mario_action(m, ACT_HOLDING_BOWSER, 0);
    }

    stationary_ground_step(m);
    return FALSE;
}

s32 act_holding_bowser(struct MarioState *m) {
    s16 spin;

    if (m->input & INPUT_B_PRESSED) {
        if (m->angleVel[1] <= -0xE00 || m->angleVel[1] >= 0xE00) {
            play_sound(SOUND_MARIO_SO_LONGA_BOWSER, m->marioObj->header.gfx.cameraToObject);
        } else {
            play_sound(SOUND_MARIO_HERE_WE_GO, m->marioObj->header.gfx.cameraToObject);
        }
        return set_mario_action(m, ACT_RELEASING_BOWSER, 0);
    }

    if (m->angleVel[1] == 0) {
        if (m->actionTimer++ > 120) {
            return set_mario_action(m, ACT_RELEASING_BOWSER, 1);
        }

        set_mario_animation(m, MARIO_ANIM_HOLDING_BOWSER);
    } else {
        m->actionTimer = 0;
        set_mario_animation(m, MARIO_ANIM_SWINGING_BOWSER);
    }

    if (m->intendedMag > 20.0f) {
        if (m->actionArg == 0) {
            m->actionArg = 1;
            m->twirlYaw = m->intendedYaw;
        } else {
            // spin = acceleration
            spin = (s16)(m->intendedYaw - m->twirlYaw) / 0x80;

            spin = CLAMP(spin, -0x80, 0x80);

            m->twirlYaw = m->intendedYaw;
            m->angleVel[1] += spin;

            m->angleVel[1] = CLAMP(m->angleVel[1], -0x1000, 0x1000);
        }
    } else {
        m->actionArg = 0;
        m->angleVel[1] = approach_s32(m->angleVel[1], 0x0, 0x40, 0x40);
    }

    // spin = starting yaw
    spin = m->faceAngle[1];
    m->faceAngle[1] += m->angleVel[1];

    // play sound on overflow
    if (m->angleVel[1] <= -0x100 && spin < m->faceAngle[1]) {
#if ENABLE_RUMBLE
        queue_rumble_data(4, 20);
#endif
        play_sound(SOUND_OBJ_BOWSER_SPINNING, m->marioObj->header.gfx.cameraToObject);
    }
    if (m->angleVel[1] >= 0x100 && spin > m->faceAngle[1]) {
#if ENABLE_RUMBLE
        queue_rumble_data(4, 20);
#endif
        play_sound(SOUND_OBJ_BOWSER_SPINNING, m->marioObj->header.gfx.cameraToObject);
    }

    stationary_ground_step(m);
    if (m->angleVel[1] >= 0) {
        m->marioObj->header.gfx.angle[0] = -m->angleVel[1];
    } else {
        m->marioObj->header.gfx.angle[0] = m->angleVel[1];
    }

    return FALSE;
}

s32 act_releasing_bowser(struct MarioState *m) {
    if (++m->actionTimer == 1) {
        if (m->actionArg == 0) {
#if ENABLE_RUMBLE
            queue_rumble_data(5, 50);
#endif
            mario_throw_held_object(m);
        } else {
#if ENABLE_RUMBLE
            queue_rumble_data(4, 50);
#endif
            mario_drop_held_object(m);
        }
    }

    m->angleVel[1] = 0;
    animated_stationary_ground_step(m, MARIO_ANIM_RELEASE_BOWSER, ACT_IDLE);
    return FALSE;
}

s32 check_common_object_cancels(struct MarioState *m) {
    f32 waterSurface = m->waterLevel - 100;
    if (m->pos[1] < waterSurface) {
        return set_water_plunge_action(m);
    }

    if (m->input & INPUT_SQUISHED) {
        return drop_and_set_mario_action(m, ACT_SQUISHED, 0);
    }

    if (m->health < 0x100) {
        return drop_and_set_mario_action(m, ACT_STANDING_DEATH, 0);
    }

    return FALSE;
}

s32 mario_execute_object_action(struct MarioState *m) {
    s32 cancel = FALSE;

    if (check_common_object_cancels(m)) {
        return TRUE;
    }

    if (mario_update_quicksand(m, 0.5f)) {
        return TRUE;
    }

    /* clang-format off */
    switch (m->action) {
        case ACT_PUNCHING:           cancel = act_punching(m);           break;
        case ACT_PICKING_UP:         cancel = act_picking_up(m);         break;
        case ACT_DIVE_PICKING_UP:    cancel = act_dive_picking_up(m);    break;
        case ACT_STOMACH_SLIDE_STOP: cancel = act_stomach_slide_stop(m); break;
        case ACT_PLACING_DOWN:       cancel = act_placing_down(m);       break;
        case ACT_THROWING:           cancel = act_throwing(m);           break;
        case ACT_HEAVY_THROW:        cancel = act_heavy_throw(m);        break;
        case ACT_PICKING_UP_BOWSER:  cancel = act_picking_up_bowser(m);  break;
        case ACT_HOLDING_BOWSER:     cancel = act_holding_bowser(m);     break;
        case ACT_RELEASING_BOWSER:   cancel = act_releasing_bowser(m);   break;
    }
    /* clang-format on */

    if (!cancel && (m->input & INPUT_IN_WATER)) {
        m->particleFlags |= PARTICLE_IDLE_WATER_WAVE;
    }

    return cancel;
}
