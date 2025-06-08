// sound_waterfall.inc.c

void bhv_waterfall_sound_loop(void) {
    play_sound(SOUND_ENV_WATERFALL2, gCurrentObject->header.gfx.cameraToObject);
}
