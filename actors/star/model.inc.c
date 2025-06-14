/********************************************************************************
	RCP_HmsItemStar [ New New Version ]
														[ Nob 19, 1995 ]
 ********************************************************************************/

#define		STAR_POLYGON_R		255
#define		STAR_POLYGON_G		255
#define		STAR_POLYGON_B		 46
#define		STAR_POLYGON_A		255


/********************************************************************************/
/*	Vertex data [common].														*/
/********************************************************************************/
static Vtx vtx_star[] = {
	{ -64,   0, 0,  0,	   0<<5, 31<<5,		STAR_POLYGON_R, STAR_POLYGON_G, STAR_POLYGON_B, STAR_POLYGON_A 	},
	{  64,	 0, 0,  0,	  31<<5, 31<<5,		STAR_POLYGON_R, STAR_POLYGON_G, STAR_POLYGON_B, STAR_POLYGON_A	},
	{  64, 128, 0,  0,	  31<<5,  0<<5,		STAR_POLYGON_R, STAR_POLYGON_G, STAR_POLYGON_B, STAR_POLYGON_A	},
	{ -64, 128, 0,  0,	   0<<5,  0<<5,		STAR_POLYGON_R, STAR_POLYGON_G, STAR_POLYGON_B, STAR_POLYGON_A	},
};

/********************************************************************************/
/*	Texture data of star coin.													*/
/********************************************************************************/
ALIGNED8 static const Texture star_coin1_txt[] = {
#include "actors/star/star_0.rgba16.inc.c"
};
ALIGNED8 static const Texture star_coin2_txt[] = {
#include "actors/star/star_1.rgba16.inc.c"
};
ALIGNED8 static const Texture star_coin3_txt[] = {
#include "actors/star/star_2.rgba16.inc.c"
};
ALIGNED8 static const Texture star_coin4_txt[] = {
#include "actors/star/star_3.rgba16.inc.c"
};
ALIGNED8 static const Texture star_coin5_txt[] = {
#include "actors/star/star_4.rgba16.inc.c"
};
ALIGNED8 static const Texture star_coin6_txt[] = {
#include "actors/star/star_5.rgba16.inc.c"
};
ALIGNED8 static const Texture star_coin7_txt[] = {
#include "actors/star/star_6.rgba16.inc.c"
};
ALIGNED8 static const Texture star_coin8_txt[] = {
#include "actors/star/star_7.rgba16.inc.c"
};

/********************************************************************************/
/*	Texture data of star dust.													*/
/********************************************************************************/
ALIGNED8 static const Texture star_dust1_txt[] = {
#include "actors/star/star_dust1_txt.rgba16.inc.c"
};

ALIGNED8 static const Texture star_dust2_txt[] = {
#include "actors/star/star_dust2_txt.rgba16.inc.c"
};

ALIGNED8 static const Texture star_dust3_txt[] = {
#include "actors/star/star_dust3_txt.rgba16.inc.c"
};

ALIGNED8 static const Texture star_dust4_txt[] = {
#include "actors/star/star_dust4_txt.rgba16.inc.c"
};

ALIGNED8 static const Texture star_dust5_txt[] = {
#include "actors/star/star_dust5_txt.rgba16.inc.c"
};

ALIGNED8 static const Texture star_dust6_txt[] = {
#include "actors/star/star_dust6_txt.rgba16.inc.c"
};

ALIGNED8 static const Texture star_dust7_txt[] = {
#include "actors/star/star_dust7_txt.rgba16.inc.c"
};

ALIGNED8 static const Texture star_dust8_txt[] = {
#include "actors/star/star_dust8_txt.rgba16.inc.c"
};

/********************************************************************************/
/*	Graphic display list of star coin.											*/
/********************************************************************************/
static Gfx gfx_star[] = {
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON),

    gsxDPLoadTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
			 G_TX_CLAMP | G_TX_NOMIRROR, G_TX_CLAMP | G_TX_NOMIRROR,
			 5, 5, G_TX_NOLOD, G_TX_NOLOD),

	gsSPVertex(&vtx_star[0], 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),

    gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPEndDisplayList()
};

/*------------------------------------------------------------------------------*/

Gfx RCP_star1[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_coin1_txt),
	gsSPBranchList(gfx_star)
};
Gfx RCP_star2[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_coin2_txt),
	gsSPBranchList(gfx_star)
};
Gfx RCP_star3[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_coin3_txt),
	gsSPBranchList(gfx_star)
};
Gfx RCP_star4[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_coin4_txt),
	gsSPBranchList(gfx_star)
};
Gfx RCP_star5[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_coin5_txt),
	gsSPBranchList(gfx_star)
};
Gfx RCP_star6[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_coin6_txt),
	gsSPBranchList(gfx_star)
};
Gfx RCP_star7[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_coin7_txt),
	gsSPBranchList(gfx_star)
};
Gfx RCP_star8[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_coin8_txt),
	gsSPBranchList(gfx_star)
};

/********************************************************************************/
/*	Graphic display list of star dust.											*/
/********************************************************************************/
static Gfx gfx_star_dust[] = {
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON),

    gsxDPLoadTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
			 G_TX_CLAMP | G_TX_NOMIRROR, G_TX_CLAMP | G_TX_NOMIRROR,
			 5, 5, G_TX_NOLOD, G_TX_NOLOD),

	gsSPVertex(&vtx_star[0], 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),

    gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPEndDisplayList()
};

/*------------------------------------------------------------------------------*/

Gfx RCP_star_dust1[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_dust1_txt),
	gsSPBranchList(gfx_star_dust)
};
Gfx RCP_star_dust2[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_dust2_txt),
	gsSPBranchList(gfx_star_dust)
};
Gfx RCP_star_dust3[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_dust3_txt),
	gsSPBranchList(gfx_star_dust)
};
Gfx RCP_star_dust4[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_dust4_txt),
	gsSPBranchList(gfx_star_dust)
};
Gfx RCP_star_dust5[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_dust5_txt),
	gsSPBranchList(gfx_star_dust)
};
Gfx RCP_star_dust6[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_dust6_txt),
	gsSPBranchList(gfx_star_dust)
};
Gfx RCP_star_dust7[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_dust7_txt),
	gsSPBranchList(gfx_star_dust)
};
Gfx RCP_star_dust8[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_dust8_txt),
	gsSPBranchList(gfx_star_dust)
};

// IGNORE THESE
const Gfx star_seg3_dl_body[] = {};
const Gfx star_seg3_dl_eyes[] = {};