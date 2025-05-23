// 0x0E000A70
const GeoLayout ttm_geo_000A70[] = {
   GEO_NODE_SCREEN_AREA(10, SCREEN_CENTER_X, SCREEN_CENTER_Y, SCREEN_CENTER_X, SCREEN_CENTER_Y),
   GEO_OPEN_NODE(),
      GEO_ZBUFFER(0),
      GEO_OPEN_NODE(),
         GEO_NODE_ORTHO(100),
         GEO_OPEN_NODE(),
            GEO_BACKGROUND(BACKGROUND_UNDERWATER_CITY, geo_skybox_main),
         GEO_CLOSE_NODE(),
      GEO_CLOSE_NODE(),
      GEO_ZBUFFER(1),
      GEO_OPEN_NODE(),
         GEO_CAMERA_FRUSTUM_WITH_FUNC(45, 100, 12800, geo_camera_fov),
         GEO_OPEN_NODE(),
            GEO_CAMERA(CAMERA_MODE_RADIAL, 0, 2000, 6000, 0, -2200, 0, geo_camera_main),
            GEO_OPEN_NODE(),
               GEO_DISPLAY_LIST(LAYER_OPAQUE, ttm_seg7_dl_0700A120),
               GEO_DISPLAY_LIST(LAYER_OPAQUE, ttm_seg7_dl_0700A2E0),
               GEO_RENDER_OBJ(),
               GEO_ASM(0,                                 geo_painting_update),
               GEO_ASM(PAINTING_ID(0, 2),                 geo_painting_draw),
               GEO_ASM(0,                                 geo_movtex_pause_control),
               GEO_ASM(MOVTEX_TTM_BEGIN_WATERFALL,        geo_movtex_draw_nocolor),
               GEO_ASM(MOVTEX_TTM_END_WATERFALL,          geo_movtex_draw_nocolor),
               GEO_ASM(MOVTEX_TTM_BEGIN_PUDDLE_WATERFALL, geo_movtex_draw_nocolor),
               GEO_ASM(MOVTEX_TTM_END_PUDDLE_WATERFALL,   geo_movtex_draw_nocolor),
               GEO_ASM(MOVTEX_TTM_PUDDLE_WATERFALL,       geo_movtex_draw_nocolor),
               GEO_ASM(TTM_MOVTEX_PUDDLE,                 geo_movtex_draw_water_regions),
               GEO_ASM(ENVFX_MODE_NONE,                   geo_envfx_main),
            GEO_CLOSE_NODE(),
         GEO_CLOSE_NODE(),
      GEO_CLOSE_NODE(),
      GEO_ZBUFFER(0),
      GEO_OPEN_NODE(),
         GEO_ASM(0, geo_cannon_circle_base),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
