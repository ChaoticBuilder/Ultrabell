/**********************************************************************
 * hms.h;
 * Used to convert Nintendo "hms" structured files into "Geo"
 * format files compatible with the Super Mario 64 decompilation project.
 * Last updated: 8th of November, 2020.
 **********************************************************************/

// -Wmissing-braces is largely an unimportant warning so we get rid of it as it slows down build time significantly.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"

#define Hierarchy GeoLayout

#define	RM_BACK				0
#define	RM_SURF				1
#define	RM_DECAL			2
#define	RM_INTER			3
#define	RM_SPRITE			4
#define	RM_XSURF			5
#define	RM_XDECAL			6
#define	RM_XINTER			7

#define SCREEN_NEAR 100
#define SCREEN_FAR 12800

#define	CAM_FIELD	1
#define	CAM_INSIDE	2
#define	CAM_WATER	3
#define	CAM_DUNGEON	4
#define	CAM_TWOPLAY	5
#define	CAM_VIEWING	6
#define	CAM_BRIDGE	7
#define	CAM_RIVER	8
#define	CAM_BACK	9
#define	CAM_CANNON	10
#define	CAM_KUPPA	11
#define	CAM_RAIL	12
#define	CAM_FIX		13
#define	CAM_PARALLEL	14
#define	CAM_SLIDER	15
#define	CAM_DUNGEON_O	16
#define	CAM_SPIRAL	17

#define LIGHT_X 40
#define LIGHT_Y 40
#define LIGHT_Z 40

#define B_SCALE 0.25
#define C_SCALE 4

// Commonly used functions.
#define ZoomControl geo_camera_fov
#define GameCamera geo_camera_main
#define WeatherProc geo_envfx_main
#define ControlShapeAnime geo_switch_anim_state
#define AlphaControl geo_update_layer_transparency
#define CtrlMarioHead geo_mario_head_rotation
#define CtrlMarioEye geo_switch_mario_eyes
#define CtrlMarioHand geo_switch_mario_hand
#define CtrlMarioTaking geo_switch_mario_hand_grab_pos
#define CtrlMarioAlpha geo_mirror_mario_set_alpha
#define CtrlMarioLOD geo_switch_mario_stand_run
#define CtrlMarioRenderMode geo_switch_mario_cap_effect
#define CtrlMarioWaist geo_move_mario_part_from_parent

#define	RGBA16(r,g,b,a)	(((r)<<11) | ((g)<<6) | ((b)<<1) | (a))
#define ShapeColor(R,G,B) {{R/C_SCALE ,G/C_SCALE ,B/C_SCALE ,0 ,R/C_SCALE ,G/C_SCALE ,B/C_SCALE ,0 },{ R,G,B,0, R,G,B,0, LIGHT_X, LIGHT_Y, LIGHT_Z, 0} }

#define hmsBegin() GEO_OPEN_NODE(),
#define hmsEnd() GEO_CLOSE_NODE(),
#define hmsScene(x, y, width, height, numEntries) GEO_NODE_SCREEN_AREA(numEntries, x, y, width, height),
#define hmsLayer(layer) GEO_ZBUFFER(layer),
#define hmsOrtho(scale) GEO_NODE_ORTHO(scale),
#define hmsClear(background, function) GEO_BACKGROUND(background, function),
#define hmsPerspective(fov, near, far, func) GEO_CAMERA_FRUSTUM_WITH_FUNC(fov, near, far, func),
#define hmsCamera(type, x1, y1, z1, x2, y2, z2, function) GEO_CAMERA(type, x1, y1, z1, x2, y2, z2, function),
#define hmsGfx(layer, displayList) GEO_DISPLAY_LIST(layer, displayList),
#define hmsObject() GEO_RENDER_OBJ(),
#define hmsCProg(param, function) GEO_ASM(param, function),
#define hmsExit() GEO_END(),
#define hmsShadow(scale, solidity, type) GEO_SHADOW(type, solidity, scale),
#define hmsScale(scale) GEO_SCALE(0x00, ((unsigned long)(scale * 65536.0f))),
#define hmsJoint(layer, displayList, x, y, z) GEO_ANIMATED_PART(layer, x, y, z, displayList),
#define hmsSelect(count, function) GEO_SWITCH_CASE(count, function),
#define hmsReturn() GEO_RETURN(),
#define hmsGroup() GEO_NODE_START(),
#define hmsRotate(ux, uy, uz) GEO_ROTATION_NODE(0x00, ux, uy, uz),
#define hmsLOD(minDistance, maxDistance) GEO_RENDER_RANGE(minDistance, maxDistance),
#define hmsCall(scriptTarget) GEO_BRANCH(1, scriptTarget),
#define hmsSucker(param, ux, uy, uz, nodeFunc) GEO_HELD_OBJECT(param, ux, uy, uz, nodeFunc),
#define hmsBboard(xTranslation, yTranslation, zTranslation) GEO_BILLBOARD(),
#define hmsTrans(ux, uy, uz) GEO_TRANSLATE_NODE(1, ux, uy, uz),
#define hmsHeader(cullingRadius) GEO_CULLING_RADIUS(cullingRadius),
