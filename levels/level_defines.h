// Define lists for list of level for macros. Each of the following fields are described:
// Argument 1: Internal ROM name of the level. 15 characters max.
// Argument 2: Level enumerator for enum used to identify the level ID.
// Argument 3: Course enumerator for enum used to identify the course ID.
// Argument 4: Shorthand name of the level which should be the name of the levels/ folder of the level.
// Argument 5: The shared texture bin used.
// Argument 6: Acoustic reaches for each levels.
// Argument 7, 8, 9: Echo levels for individual areas.  < Clueless hackers should change these for their levels btw
// Argument 10: Specify dynamic music tables for levels, if specified. _ for none.
// Argument 11: Specify level camera table, if specified. _ for none.

// NOTE: Be sure to edit sZoomOutAreaMasks in camera.c, as there isnt a good way to macro those right now.
// TODO: Figure something out for sZoomOutAreaMasks?

// todo: remove the stub levels for new levels in the future
// also shoutouts to yandex translate it's goated fr

// Bomb
// GOSUTO MANOR

STUB_LEVEL("", LEVEL_UNKNOWN_1, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) // CASTLE
STUB_LEVEL("", LEVEL_UNKNOWN_2, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) // ATHLETIC
STUB_LEVEL("", LEVEL_UNKNOWN_3, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) // DONJON
DEFINE_LEVEL(", KYASTU POCHI", LEVEL_CASTLE_GROUNDS, COURSE_NONE, castle_grounds, outside, 25000, 0x08, 0x08, 0x08, _, _) // Castle Porch
DEFINE_LEVEL(", KOTYADO", LEVEL_CASTLE_COURTYARD, COURSE_NONE, castle_courtyard, outside, 20000, 0x08, 0x08, 0x08, _, _) // Courtyard
DEFINE_LEVEL(", PEACH KYASTU", LEVEL_CASTLE, COURSE_NONE, castle_inside, inside, 20000, 0x00, 0x00, 0x00, _, sCamCastle) // Peach's Palace
DEFINE_LEVEL("1 BAKUBAKU HIRO", LEVEL_BOB, COURSE_BOB, bob, generic, 20000, 0x08, 0x00, 0x00, _, sCamBOB) // "Bom-Bom" Plains
DEFINE_LEVEL("2 FOTO YAMA", LEVEL_WF, COURSE_WF, wf, grass, 13000, 0x08, 0x08, 0x08, _, _) // Fortress Mountain
DEFINE_LEVEL("3 MINO SHIOA", LEVEL_JRB, COURSE_JRB, jrb, water, 20000, 0x10, 0x18, 0x18, sDynJrb, _) // Ocean Shore
DEFINE_LEVEL("4 YUKI SLIDER", LEVEL_CCM, COURSE_CCM, ccm, snow, 17000, 0x10, 0x38, 0x38, _, sCamCCM) // Snow Slider
DEFINE_LEVEL("5 OBAKE MANO", LEVEL_BBH, COURSE_BBH, bbh, spooky, 28000, 0x28, 0x28, 0x28, sDynBbh, sCamBBH) // Boo Mansion
DEFINE_LEVEL("6 DUNJON DOKVU", LEVEL_HMC, COURSE_HMC, hmc, cave, 16000, 0x28, 0x28, 0x28, sDynHmc, sCamHMC) // Dungeon Cave
DEFINE_LEVEL("7 FYKAJI", LEVEL_LLL, COURSE_LLL, lll, fire, 22000, 0x08, 0x30, 0x30, _, _) // Fire Land
DEFINE_LEVEL("8 SANAJI", LEVEL_SSL, COURSE_SSL, ssl, generic, 15000, 0x08, 0x30, 0x30, _, sCamSSL) // Desert Land
DEFINE_LEVEL("9 OSHIAN TOCHI", LEVEL_DDD, COURSE_DDD, ddd, water, 17000, 0x10, 0x20, 0x20, sDynDdd, _) // Ocean Land
DEFINE_LEVEL("A YUKIDARUMA", LEVEL_SL, COURSE_SL, sl, snow, 14000, 0x10, 0x28, 0x28, _, sCamSL) // Snowman
DEFINE_LEVEL("B POOL", LEVEL_WDW, COURSE_WDW, wdw, grass, 17000, 0x10, 0x18, 0x18, sDynWdw, _) // Ocean Pyramid
DEFINE_LEVEL("C OOKII YAMA", LEVEL_TTM, COURSE_TTM, ttm, mountain, 15000, 0x08, 0x08, 0x08, _, _) // Tall Mountain
DEFINE_LEVEL("D KYODAI SEKAI", LEVEL_THI, COURSE_THI, thi, grass, 20000, 0x0c, 0x0c, 0x20, _, sCamTHI) // Giant World
DEFINE_LEVEL("E NAIBUKUROKKU", LEVEL_TTC, COURSE_TTC, ttc, machine, 18000, 0x18, 0x18, 0x18, _, _) // Internal Clock
DEFINE_LEVEL("F HIKOUSEN", LEVEL_RR, COURSE_RR, rr, sky, 20000, 0x20, 0x20, 0x20, _, sCamRR) // Airship
DEFINE_LEVEL("^ DAKU KUPPA", LEVEL_BITDW, COURSE_BITDW, bitdw, sky, 16000, 0x28, 0x28, 0x28, _, _) // Dark World Bowser
DEFINE_LEVEL("^ KAJI KUPPA", LEVEL_BITFS, COURSE_BITFS, bitfs, sky, 16000, 0x28, 0x28, 0x28, _, _) // Fire Sea Bowser
DEFINE_LEVEL("^ SUKAI KUPPA", LEVEL_BITS, COURSE_BITS, bits, sky, 16000, 0x28, 0x28, 0x28, _, _) // Sky Bowser
DEFINE_LEVEL("~ UINGU KYAP", LEVEL_TOTWC, COURSE_TOTWC, totwc, sky, 20000, 0x20, 0x20, 0x20, _, _) // Wing Cap
DEFINE_LEVEL("~ GOSUTO KYAP", LEVEL_VCUTM, COURSE_VCUTM, vcutm, outside, 30000, 0x28, 0x28, 0x28, _, _) // Vanish Cap
DEFINE_LEVEL("~ KINZOKU KYAP", LEVEL_COTMC, COURSE_COTMC, cotmc, cave, 18000, 0x28, 0x28, 0x28, _, sCamCotMC) // Metal Cap
DEFINE_LEVEL(", PEACH SLIDE", LEVEL_PSS, COURSE_PSS, pss, mountain, 20000, 0x28, 0x28, 0x28, _, _) // Peach's Slide
DEFINE_LEVEL(", AKUARIUMU", LEVEL_SA, COURSE_SA, sa, inside, 20000, 0x10, 0x10, 0x10, _, _) // Aquarium
DEFINE_LEVEL(", UINGU MARIO", LEVEL_WMOTR, COURSE_WMOTR, wmotr, generic, 20000, 0x28, 0x28, 0x28, _, _) // Wing Mario
DEFINE_LEVEL("| KUPPA 1", LEVEL_BOWSER_1, COURSE_BITDW, bowser_1, generic, 60000, 0x40, 0x40, 0x40, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_32, COURSE_NONE, 20000, 0x70, 0x00, 0x00, _, _) 
DEFINE_LEVEL("| KUPPA 2", LEVEL_BOWSER_2, COURSE_BITFS, bowser_2, fire, 60000, 0x40, 0x40, 0x40, _, _) 
DEFINE_LEVEL("| KUPPA 3", LEVEL_BOWSER_3, COURSE_BITS, bowser_3, generic, 60000, 0x40, 0x40, 0x40, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_35, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
DEFINE_LEVEL("~ END CAKE", LEVEL_ENDING, COURSE_CAKE_END, ending, generic, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_37, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_38, COURSE_NONE, 20000, 0x00, 0x00, 0x00, sDynUnk38, _) 