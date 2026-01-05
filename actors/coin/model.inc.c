#include "config.h"

// Coin

#ifdef IA8_30FPS_COINS

// YELLOW
// 0x030056C0
static const Vtx coin_seg3_vertex_yellow[] = {
    {{{   -32,      0,      0}, 0, {   -16,   4080}, {0x00, 0x00, 0x00, 0xff}}},
    {{{    32,      0,      0}, 0, {  4080,   4080}, {0x00, 0x00, 0x00, 0xff}}},
    {{{    32,     64,      0}, 0, {  4080,    -16}, {0x00, 0x00, 0x00, 0xff}}},
    {{{   -32,     64,      0}, 0, {   -16,    -16}, {0x00, 0x00, 0x00, 0xff}}},
};

// BLUE
// 0x03005700
static const Vtx coin_seg3_vertex_blue[] = {
    {{{   -48,      0,      0}, 0, {   -16,   4080}, {0x80, 0x80, 0xff, 0xff}}},
    {{{    48,      0,      0}, 0, {  4080,   4080}, {0x80, 0x80, 0xff, 0xff}}},
    {{{    48,     96,      0}, 0, {  4080,    -16}, {0x80, 0x80, 0xff, 0xff}}},
    {{{   -48,     96,      0}, 0, {   -16,    -16}, {0x80, 0x80, 0xff, 0xff}}},
};

// RED
// 0x03005740
static const Vtx coin_seg3_vertex_red[] = {
    {{{   -36,      0,      0}, 0, {   -16,   4080}, {0xff, 0x00, 0x00, 0xff}}},
    {{{    36,      0,      0}, 0, {  4080,   4080}, {0xff, 0x00, 0x00, 0xff}}},
    {{{    36,     72,      0}, 0, {  4080,    -16}, {0xff, 0x00, 0x00, 0xff}}},
    {{{   -36,     72,      0}, 0, {   -16,    -16}, {0xff, 0x00, 0x00, 0xff}}},
};

// SECRET
// 0x03005780
static const Vtx coin_seg3_vertex_secret[] = {
    {{{   -36,      0,      0}, 0, {   -16,   4080}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    36,      0,      0}, 0, {  4080,   4080}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    36,     72,      0}, 0, {  4080,    -16}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   -36,     72,      0}, 0, {   -16,    -16}, {0xff, 0xff, 0xff, 0xff}}},
};

// YELLOW REV
// 0x030056C0
static const Vtx coin_seg3_vertex_yellow_r[] = {
    {{{   -32,      0,      0}, 0, {  4080,   4080}, {0xff, 0x80, 0x00, 0xff}}},
    {{{    32,      0,      0}, 0, {   -16,   4080}, {0xff, 0x80, 0x00, 0xff}}},
    {{{    32,     64,      0}, 0, {   -16,    -16}, {0xff, 0x80, 0x00, 0xff}}},
    {{{   -32,     64,      0}, 0, {  4080,    -16}, {0xff, 0x80, 0x00, 0xff}}},
};

// BLUE REV
// 0x03005700
static const Vtx coin_seg3_vertex_blue_r[] = {
    {{{   -48,      0,      0}, 0, {  4080,   4080}, {0x80, 0x80, 0xff, 0xff}}},
    {{{    48,      0,      0}, 0, {   -16,   4080}, {0x80, 0x80, 0xff, 0xff}}},
    {{{    48,     96,      0}, 0, {   -16,    -16}, {0x80, 0x80, 0xff, 0xff}}},
    {{{   -48,     96,      0}, 0, {  4080,    -16}, {0x80, 0x80, 0xff, 0xff}}},
};

// RED REV
// 0x03005740
static const Vtx coin_seg3_vertex_red_r[] = {
    {{{   -36,      0,      0}, 0, {  4080,   4080}, {0xff, 0x00, 0x00, 0xff}}},
    {{{    36,      0,      0}, 0, {   -16,   4080}, {0xff, 0x00, 0x00, 0xff}}},
    {{{    36,     72,      0}, 0, {   -16,    -16}, {0xff, 0x00, 0x00, 0xff}}},
    {{{   -36,     72,      0}, 0, {  4080,    -16}, {0xff, 0x00, 0x00, 0xff}}},
};

// SECRET REV
// 0x03005780
static const Vtx coin_seg3_vertex_secret_r[] = {
    {{{   -36,      0,      0}, 0, {  4080,   4080}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    36,      0,      0}, 0, {   -16,   4080}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    36,     72,      0}, 0, {   -16,    -16}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   -36,     72,      0}, 0, {  4080,    -16}, {0xff, 0xff, 0xff, 0xff}}},
};

// 0x03005780
ALIGNED8 static const Texture coin_seg3_texture_0_ia8[] = {
#include "actors/coin/coin_0.ia8.inc.c"
};

// 0x03005F80
ALIGNED8 static const Texture coin_seg3_texture_22_5_ia8[] = {
#include "actors/coin/coin_22_5.ia8.inc.c"
};

// 0x03006780
ALIGNED8 static const Texture coin_seg3_texture_45_ia8[] = {
#include "actors/coin/coin_45.ia8.inc.c"
};

// 0x03006F80
ALIGNED8 static const Texture coin_seg3_texture_67_5_ia8[] = {
#include "actors/coin/coin_67_5.ia8.inc.c"
};

// 0x03007
ALIGNED8 static const Texture coin_seg3_texture_90_ia8[] = {
#include "actors/coin/coin_90.ia8.inc.c"
};

// set geo
// 0x03007780 - 0x030077D0
const Gfx coin_seg3_dl_start[] = {
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA),
    gsSPTexture(32767, 32767, 0, G_TX_RENDERTILE, G_ON),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_8b, 8, 0, G_TX_LOADTILE, 0, (G_TX_CLAMP | G_TX_NOMIRROR), 6, G_TX_NOLOD, (G_TX_CLAMP | G_TX_NOMIRROR), 6, G_TX_NOLOD),
    gsDPLoadSync(),
    gsDPLoadTile(G_TX_LOADTILE, 0, 0, 252, 252),
    gsDPPipeSync(),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_8b, 8, 0, G_TX_RENDERTILE, 0, (G_TX_CLAMP | G_TX_NOMIRROR), 6, G_TX_NOLOD, (G_TX_CLAMP | G_TX_NOMIRROR), 6, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, 252, 252),
    gsSPEndDisplayList(),
};

// clear geo
// 0x030077D0 - 0x03007800
const Gfx coin_seg3_dl_end[] = {
    gsSP2Triangles( 0,  1,  2, 0x0,  0,  2,  3, 0x0),
    gsSPTexture(0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

// YELLOW
const Gfx coin_seg3_dl_yellow_0[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_0_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_yellow, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_yellow_22_5[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_22_5_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_yellow, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_yellow_45[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_45_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_yellow, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_yellow_67_5[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_67_5_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_yellow, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_yellow_90[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_90_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_yellow, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_yellow_67_5_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_67_5_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_yellow_r, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_yellow_45_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_45_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_yellow_r, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_yellow_22_5_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_22_5_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_yellow_r, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

// BLUE
const Gfx coin_seg3_dl_blue_0[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_0_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_blue, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_blue_22_5[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_22_5_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_blue, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_blue_45[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_45_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_blue, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_blue_67_5[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_67_5_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_blue, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_blue_90[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_90_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_blue, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_blue_67_5_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_67_5_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_blue_r, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_blue_22_5_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_22_5_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_blue_r, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_blue_45_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_45_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_blue_r, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

// RED
const Gfx coin_seg3_dl_red_0[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_0_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_red, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_red_22_5[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_22_5_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_red, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_red_45[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_45_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_red, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_red_67_5[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_67_5_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_red, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_red_90[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_90_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_red, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_red_67_5_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_67_5_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_red_r, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_red_45_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_45_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_red_r, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};


const Gfx coin_seg3_dl_red_22_5_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_22_5_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_red_r, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};
// SECRET
const Gfx coin_seg3_dl_secret_0[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_0_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_secret, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_secret_22_5[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_22_5_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_secret, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_secret_45[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_45_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_secret, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_secret_67_5[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_67_5_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_secret, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_secret_90[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_90_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_secret, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_secret_67_5_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_67_5_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_secret_r, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

const Gfx coin_seg3_dl_secret_45_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_45_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_secret_r, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};


const Gfx coin_seg3_dl_secret_22_5_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b, 64, coin_seg3_texture_22_5_ia8),
    gsSPDisplayList(coin_seg3_dl_start),
    gsSPVertex(coin_seg3_vertex_secret_r, 4, 0),
    gsSPBranchList(coin_seg3_dl_end),
};

#else
static const Vtx coin_seg3_vertex_yellow[] = {
    {{{   -32,      0,      0}, 0, {    -8,   2040}, {0xff, 0xff, 0xff, 0xff}}}, // bottom left + small diagonal stripe
    {{{    32,      0,      0}, 0, {  2040,   2040}, {0xff, 0xff, 0xff, 0xff}}}, // bottom right
    {{{    32,     64,      0}, 0, {  2040,     -8}, {0xff, 0xff, 0xff, 0xff}}}, // top right + big diagonal stripe
    {{{   -32,     64,      0}, 0, {    -8,     -8}, {0xff, 0xff, 0xff, 0xff}}}, // top left
};

static const Vtx coin_seg3_vertex_yellow_r[] = {
    {{{   -32,      0,      0}, 0, {  2040,   2040}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    32,      0,      0}, 0, {    -8,   2040}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    32,     64,      0}, 0, {    -8,     -8}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   -32,     64,      0}, 0, {  2040,     -8}, {0xff, 0xff, 0xff, 0xff}}},
};

static const Vtx coin_seg3_vertex_red[] = {
    {{{   -40,      0,      0}, 0, {    -8,   2040}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    40,      0,      0}, 0, {  2040,   2040}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    40,     80,      0}, 0, {  2040,     -8}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   -40,     80,      0}, 0, {    -8,     -8}, {0xff, 0xff, 0xff, 0xff}}},
};

static const Vtx coin_seg3_vertex_red_r[] = {
    {{{   -40,      0,      0}, 0, {  2040,   2040}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    40,      0,      0}, 0, {    -8,   2040}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    40,     80,      0}, 0, {    -8,     -8}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   -40,     80,      0}, 0, {  2040,     -8}, {0xff, 0xff, 0xff, 0xff}}},
};

static const Vtx coin_seg3_vertex_blue[] = {
    {{{   -48,      0,      0}, 0, {    -8,   2040}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    48,      0,      0}, 0, {  2040,   2040}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    48,     96,      0}, 0, {  2040,     -8}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   -48,     96,      0}, 0, {    -8,     -8}, {0xff, 0xff, 0xff, 0xff}}},
};

static const Vtx coin_seg3_vertex_blue_r[] = {
    {{{   -48,      0,      0}, 0, {  2040,   2040}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    48,      0,      0}, 0, {    -8,   2040}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    48,     96,      0}, 0, {    -8,     -8}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   -48,     96,      0}, 0, {  2040,     -8}, {0xff, 0xff, 0xff, 0xff}}},
};

/* YELLOW */
ALIGNED8 static const Texture coin_yellow_0[] = {
#include "actors/coin/coin_0.rgba16.inc.c"
};

ALIGNED8 static const Texture coin_yellow_1[] = {
#include "actors/coin/coin_1.rgba16.inc.c"
};

ALIGNED8 static const Texture coin_yellow_2[] = {
#include "actors/coin/coin_2.rgba16.inc.c"
};

ALIGNED8 static const Texture coin_yellow_3[] = {
#include "actors/coin/coin_3.rgba16.inc.c"
};

/* RED */
ALIGNED8 static const Texture coin_red_0[] = {
#include "actors/coin/coin_red_0.rgba16.inc.c"
};

ALIGNED8 static const Texture coin_red_1[] = {
#include "actors/coin/coin_red_1.rgba16.inc.c"
};

ALIGNED8 static const Texture coin_red_2[] = {
#include "actors/coin/coin_red_2.rgba16.inc.c"
};

ALIGNED8 static const Texture coin_red_3[] = {
#include "actors/coin/coin_red_3.rgba16.inc.c"
};

/* BLUE */
ALIGNED8 static const Texture coin_blue_0[] = {
#include "actors/coin/coin_blue_0.rgba16.inc.c"
};

ALIGNED8 static const Texture coin_blue_1[] = {
#include "actors/coin/coin_blue_1.rgba16.inc.c"
};

ALIGNED8 static const Texture coin_blue_2[] = {
#include "actors/coin/coin_blue_2.rgba16.inc.c"
};

ALIGNED8 static const Texture coin_blue_3[] = {
#include "actors/coin/coin_blue_3.rgba16.inc.c"
};

// 0x03007780 - 0x030077D0
const Gfx coin_seg3_sub_dl_begin[] = {
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 32 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (32 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsSPEndDisplayList(),
};

// 0x030077D0 - 0x03007800
const Gfx coin_seg3_sub_dl_end[] = {
    gsSP2Triangles( 0,  1,  2, 0x0,  0,  2,  3, 0x0),
    gsSPTexture(0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

// 0x03007800 - 0x03007828
const Gfx coin_seg3_dl_yellow_0[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_yellow_0),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_yellow, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};

// 0x03007828 - 0x03007850
const Gfx coin_seg3_dl_yellow_1[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_yellow_1),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_yellow, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};

// 0x03007850 - 0x03007878
const Gfx coin_seg3_dl_yellow_2[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_yellow_2),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_yellow, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};

// 0x03007878 - 0x030078A0
const Gfx coin_seg3_dl_yellow_3[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_yellow_3),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_yellow, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};

const Gfx coin_seg3_dl_yellow_2_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_yellow_2),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_yellow_r, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};

const Gfx coin_seg3_dl_yellow_1_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_yellow_1),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_yellow_r, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};

// 0x03007800 - 0x03007828
const Gfx coin_seg3_dl_red_0[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_red_0),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_red, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};

// 0x03007828 - 0x03007850
const Gfx coin_seg3_dl_red_1[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_red_1),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_red, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};

// 0x03007850 - 0x03007878
const Gfx coin_seg3_dl_red_2[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_red_2),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_red, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};

// 0x03007878 - 0x030078A0
const Gfx coin_seg3_dl_red_3[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_red_3),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_red, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};

const Gfx coin_seg3_dl_red_2_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_red_2),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_red_r, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};

const Gfx coin_seg3_dl_red_1_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_red_1),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_red_r, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};

// 0x03007800 - 0x03007828
const Gfx coin_seg3_dl_blue_0[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_blue_0),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_blue, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};

// 0x03007828 - 0x03007850
const Gfx coin_seg3_dl_blue_1[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_blue_1),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_blue, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};

// 0x03007850 - 0x03007878
const Gfx coin_seg3_dl_blue_2[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_blue_2),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_blue, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};

// 0x03007878 - 0x030078A0
const Gfx coin_seg3_dl_blue_3[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_blue_3),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_blue, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};

const Gfx coin_seg3_dl_blue_2_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_blue_2),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_blue_r, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};

const Gfx coin_seg3_dl_blue_1_r[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, coin_blue_1),
    gsSPDisplayList(coin_seg3_sub_dl_begin),
    gsSPVertex(coin_seg3_vertex_blue_r, 4, 0),
    gsSPBranchList(coin_seg3_sub_dl_end),
};
#endif