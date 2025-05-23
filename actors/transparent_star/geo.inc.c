
extern Gfx  gfx_dotstar[];
extern Gfx  gfx_shadestar[];

const GeoLayout transparent_star_geo[] = { 

	hmsShadow(100, 155, 1)
    	hmsBegin()
			hmsGfx(RM_SPRITE, gfx_dotstar)
		hmsEnd()
  	hmsExit()

};



