// ddd_sub.inc.c

void bhv_bowsers_sub_init(void) {
    load_object_static_model();
}
void bhv_bowsers_sub_door(void) {
    if (gCurrActNum != 1) {
        obj_mark_for_deletion(o);
    } else {
        load_object_static_model();
    }
}