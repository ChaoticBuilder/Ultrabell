#include <ultra64.h>

#include "sm64.h"
#include "game_init.h"
#include "main.h"
#include "audio/external.h"
#include "engine/math_util.h"
#include "area.h"
#include "level_update.h"
#include "save_file.h"
#include "sound_init.h"
#include "level_table.h"
#include "course_table.h"
#include "level_commands.h"
#include "rumble_init.h"
#include "config.h"
#include "emutest.h"
#ifdef SRAM
#include "sram.h"
#endif
#include "puppycam2.h"

#ifdef UNIQUE_SAVE_DATA
u16 MENU_DATA_MAGIC = 0x4849;
u16 SAVE_FILE_MAGIC = 0x4441;
#else
#define MENU_DATA_MAGIC 0x4849
#define SAVE_FILE_MAGIC 0x4441
#endif

//STATIC_ASSERT(sizeof(struct SaveBuffer) == EEPROM_SIZE, "eeprom buffer size must match");

extern struct SaveBuffer gSaveBuffer;

struct WarpCheckpoint gWarpCheckpoint;

s8 gMainMenuDataModified;
s8 gSaveFileModified;

u8 gLastCompletedCourseNum = COURSE_NONE;
u8 gLastCompletedStarNum = 0;
s8 sUnusedGotGlobalCoinHiScore = FALSE;
u8 gGotFileCoinHiScore = FALSE;
u8 gCurrCourseStarFlags = 0;

u8 gSpecialTripleJump = FALSE;

#define STUB_LEVEL(_0, _1, courseenum, _3, _4, _5, _6, _7, _8) courseenum,
#define DEFINE_LEVEL(_0, _1, courseenum, _3, _4, _5, _6, _7, _8, _9, _10) courseenum,

s8 gLevelToCourseNumTable[] = {
    #include "levels/level_defines.h"
};
#undef STUB_LEVEL
#undef DEFINE_LEVEL

STATIC_ASSERT(ARRAY_COUNT(gLevelToCourseNumTable) == LEVEL_COUNT - 1,
              "change this array if you are adding levels");
#ifdef EEP
#include "vc_ultra.h"

/**
 * Read from EEPROM to a given address.
 * The EEPROM address is computed using the offset of the destination address from gSaveBuffer.
 * Try at most 4 times, and return 0 on success. On failure, return the status returned from
 * osEepromLongRead. It also returns 0 if EEPROM isn't loaded correctly in the system.
 */
static s32 read_eeprom_data(void *buffer, s32 size) {
    s32 status = 0;

    if (gEepromProbe != 0) {
        s32 triesLeft = 4;
        u32 offset = (u32)((u8 *) buffer - (u8 *) &gSaveBuffer) / 8;

        do {
#if ENABLE_RUMBLE
            block_until_rumble_pak_free();
#endif
            triesLeft--;
            status = (gEmulator & EMU_WIIVC)
                   ? osEepromLongReadVC(&gSIEventMesgQueue, offset, buffer, size)
                   : osEepromLongRead  (&gSIEventMesgQueue, offset, buffer, size);
#if ENABLE_RUMBLE
            release_rumble_pak_control();
#endif
        } while (triesLeft > 0 && status != 0);
    }

    return status;
}

/**
 * Write data to EEPROM.
 * The EEPROM address is computed using the offset of the source address from gSaveBuffer.
 * Try at most 4 times, and return 0 on success. On failure, return the status returned from
 * osEepromLongWrite. Unlike read_eeprom_data, return 1 if EEPROM isn't loaded.
 */
static s32 write_eeprom_data(void *buffer, s32 size) {
    s32 status = 1;

    if (gEepromProbe != 0) {
        s32 triesLeft = 4;
        u32 offset = (u32)((u8 *) buffer - (u8 *) &gSaveBuffer) >> 3;

        do {
#if ENABLE_RUMBLE
            block_until_rumble_pak_free();
#endif
            triesLeft--;
            status = (gEmulator & EMU_WIIVC)
                   ? osEepromLongWriteVC(&gSIEventMesgQueue, offset, buffer, size)
                   : osEepromLongWrite  (&gSIEventMesgQueue, offset, buffer, size);
#if ENABLE_RUMBLE
            release_rumble_pak_control();
#endif
        } while (triesLeft > 0 && status != 0);
    }

    return status;
}
#endif
#ifdef SRAM
/**
 * Read from SRAM to a given address.
 * The SRAM address is computed using the offset of the destination address from gSaveBuffer.
 * Try at most 4 times, and return 0 on success. On failure, return the status returned from
 * nuPiReadSram. It also returns 0 if SRAM isn't loaded correctly in the system.
 */
static s32 read_eeprom_data(void *buffer, s32 size) {
    s32 status = 0;

    if (gSramProbe != 0) {
        s32 triesLeft = 4;
        u32 offset = (u32)((u8 *) buffer - (u8 *) &gSaveBuffer);

        do {
#if ENABLE_RUMBLE
            block_until_rumble_pak_free();
#endif
            triesLeft--;
            status = nuPiReadSram(offset, buffer, ALIGN4(size));
#if ENABLE_RUMBLE
            release_rumble_pak_control();
#endif
        } while (triesLeft > 0 && status != 0);
    }

    return status;
}

/**
 * Write data to SRAM.
 * The SRAM address is computed using the offset of the source address from gSaveBuffer.
 * Try at most 4 times, and return 0 on success. On failure, return the status returned from
 * nuPiWriteSram. Unlike read_eeprom_data, return 1 if SRAM isn't loaded.
 */
static s32 write_eeprom_data(void *buffer, s32 size) {
    s32 status = 1;

    if (gSramProbe != 0) {
        s32 triesLeft = 4;
        u32 offset = (u32)((u8 *) buffer - (u8 *) &gSaveBuffer);

        do {
#if ENABLE_RUMBLE
            block_until_rumble_pak_free();
#endif
            triesLeft--;
            status = nuPiWriteSram(offset, buffer, ALIGN4(size));
#if ENABLE_RUMBLE
            release_rumble_pak_control();
#endif
        } while (triesLeft > 0 && status != 0);
    }

    return status;
}
#endif


/**
 * Sum the bytes in data to data + size - 2. The last two bytes are ignored
 * because that is where the checksum is stored.
 */
static u16 calc_checksum(u8 *data, s32 size) {
    u16 chksum = 0;

    while (size-- > 2) {
        chksum += *data++;
    }
    return chksum;
}

/**
 * Verify the signature at the end of the block to check if the data is valid.
 */
static s32 verify_save_block_signature(void *buffer, s32 size, u16 magic) {
    struct SaveBlockSignature *sig = (struct SaveBlockSignature *) ((size - 4) + (u8 *) buffer);

    if (sig->magic != magic) {
        return FALSE;
    }
    if (sig->chksum != calc_checksum(buffer, size)) {
        return FALSE;
    }
    return TRUE;
}

/**
 * Write a signature at the end of the block to make sure the data is valid
 */
static void add_save_block_signature(void *buffer, s32 size, u16 magic) {
    struct SaveBlockSignature *sig = (struct SaveBlockSignature *) ((size - 4) + (u8 *) buffer);

    sig->magic = magic;
    sig->chksum = calc_checksum(buffer, size);
}

static void save_main_menu_data(void) {
    if (gMainMenuDataModified) {
        // Compute checksum
        add_save_block_signature(&gSaveBuffer.menuData, sizeof(gSaveBuffer.menuData), MENU_DATA_MAGIC);

        // Write to EEPROM
        write_eeprom_data(&gSaveBuffer.menuData, sizeof(gSaveBuffer.menuData));

        gMainMenuDataModified = FALSE;
    }
}

static void wipe_main_menu_data(void) {
    bzero(&gSaveBuffer.menuData, sizeof(gSaveBuffer.menuData));

    // Set score ages for all courses to 3, 2, 1, and 0, respectively.
    gSaveBuffer.menuData.coinScoreAges[0] = 0x3FFFFFFF;
    gSaveBuffer.menuData.coinScoreAges[1] = 0x2AAAAAAA;
    gSaveBuffer.menuData.coinScoreAges[2] = 0x15555555;

    gMainMenuDataModified = TRUE;
    save_main_menu_data();
}

static s32 get_coin_score_age(s32 fileIndex, s32 courseIndex) {
    return (gSaveBuffer.menuData.coinScoreAges[fileIndex] >> (2 * courseIndex)) & 0x3;
}

static void set_coin_score_age(s32 fileIndex, s32 courseIndex, s32 age) {
    s32 mask = 0x3 << (2 * courseIndex);

    gSaveBuffer.menuData.coinScoreAges[fileIndex] &= ~mask;
    gSaveBuffer.menuData.coinScoreAges[fileIndex] |= age << (2 * courseIndex);
}

/**
 * Mark a coin score for a save file as the newest out of all save files.
 */
static void touch_coin_score_age(s32 fileIndex, s32 courseIndex) {
    s32 i;
    u32 age;
    u32 currentAge = get_coin_score_age(fileIndex, courseIndex);

    if (currentAge != 0) {
        for (i = 0; i < NUM_SAVE_FILES; i++) {
            age = get_coin_score_age(i, courseIndex);
            if (age < currentAge) {
                set_coin_score_age(i, courseIndex, age + 1);
            }
        }

        set_coin_score_age(fileIndex, courseIndex, 0);
        gMainMenuDataModified = TRUE;
    }
}

/**
 * Mark all coin scores for a save file as new.
 */
static void touch_high_score_ages(s32 fileIndex) {
    s32 i;

    for (i = COURSE_NUM_TO_INDEX(COURSE_MIN); i <= COURSE_NUM_TO_INDEX(COURSE_STAGES_MAX); i++) {
        touch_coin_score_age(fileIndex, i);
    }
}

/**
 * Copy save file data from one backup slot to the other slot.
 */
static void restore_save_file_data(s32 fileIndex, s32 srcSlot) {
    s32 destSlot = srcSlot ^ 1;

    // Compute checksum on source data
    add_save_block_signature(&gSaveBuffer.files[fileIndex][srcSlot],
                             sizeof(gSaveBuffer.files[fileIndex][srcSlot]), SAVE_FILE_MAGIC);

    // Copy source data to destination slot
    bcopy(&gSaveBuffer.files[fileIndex][srcSlot], &gSaveBuffer.files[fileIndex][destSlot],
          sizeof(gSaveBuffer.files[fileIndex][destSlot]));

    // Write destination data to EEPROM
    write_eeprom_data(&gSaveBuffer.files[fileIndex][destSlot],
                      sizeof(gSaveBuffer.files[fileIndex][destSlot]));
}

void save_file_do_save(s32 fileIndex) {
    if (gSaveFileModified) {
        // Compute checksum
        add_save_block_signature(&gSaveBuffer.files[fileIndex][0],
                                 sizeof(gSaveBuffer.files[fileIndex][0]), SAVE_FILE_MAGIC);

        // Copy to backup slot
        bcopy(&gSaveBuffer.files[fileIndex][0], &gSaveBuffer.files[fileIndex][1],
              sizeof(gSaveBuffer.files[fileIndex][1]));

        // Write to EEPROM
        write_eeprom_data(&gSaveBuffer.files[fileIndex], sizeof(gSaveBuffer.files[fileIndex]));

        gSaveFileModified = FALSE;
    }

    save_main_menu_data();
}

void save_file_erase(s32 fileIndex) {
    touch_high_score_ages(fileIndex);
    bzero(&gSaveBuffer.files[fileIndex][0], sizeof(gSaveBuffer.files[fileIndex][0]));

    gSaveFileModified = TRUE;
    save_file_do_save(fileIndex);
}

void save_file_copy(s32 srcFileIndex, s32 destFileIndex) {
    touch_high_score_ages(destFileIndex);
    bcopy(&gSaveBuffer.files[srcFileIndex][0], &gSaveBuffer.files[destFileIndex][0],
          sizeof(gSaveBuffer.files[destFileIndex][0]));

    gSaveFileModified = TRUE;
    save_file_do_save(destFileIndex);
}

#ifdef UNIQUE_SAVE_DATA
// This should only be called once on boot and never again.
static void calculate_unique_save_magic(void) {
    u16 checksum = 0;

    for (s32 i = 0; i < 20; i++) {
        checksum += (u16) INTERNAL_ROM_NAME[i] << (i & 0x07);
    }

    MENU_DATA_MAGIC += checksum;
    SAVE_FILE_MAGIC += checksum;
}
#endif

void save_file_load_all(void) {
    s32 file;
    s32 validSlots;

#ifdef UNIQUE_SAVE_DATA
    calculate_unique_save_magic(); // This should only be called once on boot and never again.
#endif

    gMainMenuDataModified = FALSE;
    gSaveFileModified = FALSE;

    bzero(&gSaveBuffer, sizeof(gSaveBuffer));
    read_eeprom_data(&gSaveBuffer, sizeof(gSaveBuffer));

    // Verify the main menu data and wipe it if invalid.
    validSlots = verify_save_block_signature(&gSaveBuffer.menuData, sizeof(gSaveBuffer.menuData), MENU_DATA_MAGIC);
    if (!validSlots)
        wipe_main_menu_data();

    for (file = 0; file < NUM_SAVE_FILES; file++) {
        // Verify the save file and create a backup copy if only one of the slots is valid.
        validSlots = verify_save_block_signature(&gSaveBuffer.files[file][0], sizeof(gSaveBuffer.files[file][0]), SAVE_FILE_MAGIC);
        validSlots |= verify_save_block_signature(&gSaveBuffer.files[file][1], sizeof(gSaveBuffer.files[file][1]), SAVE_FILE_MAGIC) << 1;
        switch (validSlots) {
            case 0: // Neither copy is correct
                save_file_erase(file);
                break;
            case 1: // Slot 0 is correct and slot 1 is incorrect
                restore_save_file_data(file, 0);
                break;
            case 2: // Slot 1 is correct and slot 0 is incorrect
                restore_save_file_data(file, 1);
                break;
        }
    }
}

#ifdef PUPPYCAM
void puppycam_get_save(void) {
    gPuppyCam.options = gSaveBuffer.menuData.saveOptions;

    gSaveBuffer.menuData.firstBoot = gSaveBuffer.menuData.firstBoot;
#ifdef WIDE
    gConfig.widescreen = save_file_get_widescreen_mode();
#endif

    puppycam_check_save();
}

void puppycam_set_save(void) {
    gSaveBuffer.menuData.saveOptions = gPuppyCam.options;

    gSaveBuffer.menuData.firstBoot = 4;

#ifdef WIDE
    save_file_set_widescreen_mode(gConfig.widescreen);
#endif

    gMainMenuDataModified = TRUE;
    save_main_menu_data();
}

void puppycam_check_save(void) {
    if (gSaveBuffer.menuData.firstBoot != 4) {
        wipe_main_menu_data();
        gSaveBuffer.menuData.firstBoot = 4;
        puppycam_default_config();
        puppycam_set_save();
    }
}
#endif

/**
 * Reload the current save file from its backup copy, which is effectively a
 * a cached copy of what has been written to EEPROM.
 * This is used after getting a game over.
 */
void save_file_reload(void) {
    // Copy save file data from backup
    bcopy(&gSaveBuffer.files[gCurrSaveFileNum - 1][1], &gSaveBuffer.files[gCurrSaveFileNum - 1][0],
          sizeof(gSaveBuffer.files[gCurrSaveFileNum - 1][0]));

    gMainMenuDataModified = FALSE;
    gSaveFileModified = FALSE;
}

/**
 * Update the current save file after collecting a star or a key.
 * If coin score is greater than the current high score, update it.
 */
void save_file_collect_star_or_key(s16 coinScore, s16 starIndex) {
    s32 fileIndex = gCurrSaveFileNum - 1;
    s32 courseIndex = COURSE_NUM_TO_INDEX(gCurrCourseNum);
#ifdef GLOBAL_STAR_IDS
    s32 starByte = COURSE_NUM_TO_INDEX(starIndex / 7);
    s32 starFlag = 1 << (starIndex % 7);
#else
    s32 starFlag = 1 << starIndex;
#endif

    gLastCompletedCourseNum = courseIndex + 1;
    gLastCompletedStarNum = starIndex + 1;
    sUnusedGotGlobalCoinHiScore = FALSE;
    gGotFileCoinHiScore = FALSE;

    if (courseIndex >= COURSE_NUM_TO_INDEX(COURSE_MIN)
        && courseIndex <= COURSE_NUM_TO_INDEX(COURSE_STAGES_MAX)) {
        //! Compares the coin score as a 16 bit value, but only writes the 8 bit
        // truncation. This can allow a high score to decrease.

        if (coinScore > ((u16) save_file_get_max_coin_score(courseIndex) & 0xFFFF)) {
            sUnusedGotGlobalCoinHiScore = TRUE;
        }

        if (coinScore > save_file_get_course_coin_score(fileIndex, courseIndex)) {
            gSaveBuffer.files[fileIndex][0].courseCoinScores[courseIndex] = coinScore;
            touch_coin_score_age(fileIndex, courseIndex);

            gGotFileCoinHiScore = TRUE;
            gSaveFileModified = TRUE;
        }
    }

    switch (gCurrLevelNum) {
        case LEVEL_BOWSER_1:
            gMarioState->numKeys++;
            if (!(save_file_get_flags() & (SAVE_FLAG_HAVE_KEY_1 | SAVE_FLAG_UNLOCKED_BASEMENT_DOOR))) {
                save_file_set_flags(SAVE_FLAG_HAVE_KEY_1);
            }
            break;

        case LEVEL_BOWSER_2:
            gMarioState->numKeys++;
            if (!(save_file_get_flags() & (SAVE_FLAG_HAVE_KEY_2 | SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR))) {
                save_file_set_flags(SAVE_FLAG_HAVE_KEY_2);
            }
            break;
    }
#ifdef GLOBAL_STAR_IDS
    if (!(save_file_get_star_flags(fileIndex, starByte) & starFlag)) {
        save_file_set_star_flags(fileIndex, starByte, starFlag);
    }
#else
    if (!(save_file_get_star_flags(fileIndex, courseIndex) & starFlag)) {
        save_file_set_star_flags(fileIndex, courseIndex, starFlag);
    }
#endif
}

s32 save_file_exists(s32 fileIndex) {
    return (gSaveBuffer.files[fileIndex][0].flags & SAVE_FLAG_FILE_EXISTS) != 0;
}

/**
 * Get the maximum coin score across all files for a course. The lower 16 bits
 * of the returned value are the score, and the upper 16 bits are the file number
 * of the save file with this score.
 */
u32 save_file_get_max_coin_score(s32 courseIndex) {
    s32 fileIndex;
    s32 maxCoinScore = -1;
    s32 maxScoreAge = -1;
    s32 maxScoreFileNum = 0;

    for (fileIndex = 0; fileIndex < NUM_SAVE_FILES; fileIndex++) {
        if (save_file_get_star_flags(fileIndex, courseIndex) != 0) {
            s32 coinScore = save_file_get_course_coin_score(fileIndex, courseIndex);
            s32 scoreAge = get_coin_score_age(fileIndex, courseIndex);

            if (coinScore > maxCoinScore || (coinScore == maxCoinScore && scoreAge > maxScoreAge)) {
                maxCoinScore = coinScore;
                maxScoreAge = scoreAge;
                maxScoreFileNum = fileIndex + 1;
            }
        }
    }
    return (maxScoreFileNum << 16) + MAX(maxCoinScore, 0);
}

#ifdef COMPLETE_SAVE_FILE
s32 save_file_get_course_star_count(UNUSED s32 fileIndex, UNUSED s32 courseIndex) {
    return 7;
}
#else
s32 save_file_get_course_star_count(s32 fileIndex, s32 courseIndex) {
    s32 i;
    s32 count = 0;
    u8 flag = 1;
    u8 starFlags = save_file_get_star_flags(fileIndex, courseIndex);

    for (i = 0; i < 7; i++, flag <<= 1) {
        if (starFlags & flag) {
            count++;
        }
    }
    return count;
}
#endif

s32 save_file_get_total_star_count(s32 fileIndex, s32 minCourse, s32 maxCourse) {
    s32 count = 0;

    // Get standard course star count.
    for (; minCourse <= maxCourse; minCourse++) {
        count += save_file_get_course_star_count(fileIndex, minCourse);
    }

    // Add castle secret star count.
    return save_file_get_course_star_count(fileIndex, COURSE_NUM_TO_INDEX(COURSE_NONE)) + count;
}

void save_file_set_flags(u32 flags) {
    gSaveBuffer.files[gCurrSaveFileNum - 1][0].flags |= (flags | SAVE_FLAG_FILE_EXISTS);
    gSaveFileModified = TRUE;
}

void save_file_clear_flags(u32 flags) {
    gSaveBuffer.files[gCurrSaveFileNum - 1][0].flags &= ~flags;
    gSaveBuffer.files[gCurrSaveFileNum - 1][0].flags |= SAVE_FLAG_FILE_EXISTS;
    gSaveFileModified = TRUE;
}

u32 save_file_get_flags(void) {
#ifdef COMPLETE_SAVE_FILE
    return (SAVE_FLAG_FILE_EXISTS            |
            SAVE_FLAG_HAVE_WING_CAP          |
            SAVE_FLAG_HAVE_METAL_CAP         |
            SAVE_FLAG_HAVE_VANISH_CAP        |
            SAVE_FLAG_UNLOCKED_BASEMENT_DOOR |
            SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR |
            SAVE_FLAG_DDD_MOVED_BACK         |
            SAVE_FLAG_MOAT_DRAINED           |
            SAVE_FLAG_UNLOCKED_PSS_DOOR      |
            SAVE_FLAG_UNLOCKED_WF_DOOR       |
            SAVE_FLAG_UNLOCKED_CCM_DOOR      |
            SAVE_FLAG_UNLOCKED_JRB_DOOR      |
            SAVE_FLAG_UNLOCKED_BITDW_DOOR    |
            SAVE_FLAG_UNLOCKED_BITFS_DOOR    |
            SAVE_FLAG_UNLOCKED_50_STAR_DOOR  |
            SAVE_FLAG_COLLECTED_TOAD_STAR_1  |
            SAVE_FLAG_COLLECTED_TOAD_STAR_2  |
            SAVE_FLAG_COLLECTED_TOAD_STAR_3  |
            SAVE_FLAG_COLLECTED_MIPS_STAR_1  |
            SAVE_FLAG_COLLECTED_MIPS_STAR_2);
#else
    if (gCurrCreditsEntry != NULL || gCurrDemoInput != NULL) {
        return 0;
    }
    return gSaveBuffer.files[gCurrSaveFileNum - 1][0].flags;
#endif
}

/**
 * Return the bitset of obtained stars in the specified course.
 * If course is COURSE_NONE, return the bitset of obtained castle secret stars.
 */
#ifdef COMPLETE_SAVE_FILE
u32 save_file_get_star_flags(UNUSED s32 fileIndex, UNUSED s32 courseIndex) {
    return 0x7F;
}
#else
u32 save_file_get_star_flags(s32 fileIndex, s32 courseIndex) {
    u32 starFlags;

    if (courseIndex == COURSE_NUM_TO_INDEX(COURSE_NONE)) {
        starFlags = SAVE_FLAG_TO_STAR_FLAG(gSaveBuffer.files[fileIndex][0].flags);
    } else {
        starFlags = gSaveBuffer.files[fileIndex][0].courseStars[courseIndex] & 0x7F;
    }

    return starFlags;
}
#endif

/**
 * Add to the bitset of obtained stars in the specified course.
 * If course is COURSE_NONE, add to the bitset of obtained castle secret stars.
 */
void save_file_set_star_flags(s32 fileIndex, s32 courseIndex, u32 starFlags) {
    if (courseIndex == COURSE_NUM_TO_INDEX(COURSE_NONE)) {
        gSaveBuffer.files[fileIndex][0].flags |= STAR_FLAG_TO_SAVE_FLAG(starFlags);
    } else {
        gSaveBuffer.files[fileIndex][0].courseStars[courseIndex] |= starFlags;
    }

    gSaveBuffer.files[fileIndex][0].flags |= SAVE_FLAG_FILE_EXISTS;
    gSaveFileModified = TRUE;
}

#ifdef COMPLETE_SAVE_FILE
s32 save_file_get_course_coin_score(UNUSED s32 fileIndex, UNUSED s32 courseIndex) {
    return MAX_NUM_COINS;
}
#else
s32 save_file_get_course_coin_score(s32 fileIndex, s32 courseIndex) {
    return gSaveBuffer.files[fileIndex][0].courseCoinScores[courseIndex];
}
#endif

/**
 * Return TRUE if the cannon is unlocked in the current course.
 */
s32 save_file_is_cannon_unlocked(void) {
#ifdef UNLOCK_ALL
    return TRUE;
#else
    return (gSaveBuffer.files[gCurrSaveFileNum - 1][0].courseStars[gCurrCourseNum] & COURSE_FLAG_CANNON_UNLOCKED) != 0;
#endif
}

/**
 * Sets the cannon status to unlocked in the current course.
 */
void save_file_set_cannon_unlocked(void) {
    gSaveBuffer.files[gCurrSaveFileNum - 1][0].courseStars[gCurrCourseNum] |= COURSE_FLAG_CANNON_UNLOCKED;
    gSaveBuffer.files[gCurrSaveFileNum - 1][0].flags |= SAVE_FLAG_FILE_EXISTS;
    gSaveFileModified = TRUE;
}

void save_file_set_cap_pos(s16 x, s16 y, s16 z) {
    struct SaveFile *saveFile = &gSaveBuffer.files[gCurrSaveFileNum - 1][0];

    saveFile->capLevel = gCurrLevelNum;
    saveFile->capArea = gCurrAreaIndex;
    vec3s_set(saveFile->capPos, x, y, z);
    save_file_set_flags(SAVE_FLAG_CAP_ON_GROUND);
}

s32 save_file_get_cap_pos(Vec3s capPos) {
    struct SaveFile *saveFile = &gSaveBuffer.files[gCurrSaveFileNum - 1][0];
    s32 flags = save_file_get_flags();

    if (saveFile->capLevel == gCurrLevelNum && saveFile->capArea == gCurrAreaIndex
        && (flags & SAVE_FLAG_CAP_ON_GROUND)) {
        vec3s_copy(capPos, saveFile->capPos);
        return TRUE;
    }
    return FALSE;
}

void save_file_set_sound_mode(u16 mode) {
    set_sound_mode(mode);
    gSaveBuffer.menuData.soundMode = mode;

    gMainMenuDataModified = TRUE;
    save_main_menu_data();
}

#ifdef WIDE
u32 save_file_get_widescreen_mode(void) {
    return gSaveBuffer.menuData.wideMode;
}

void save_file_set_widescreen_mode(u8 mode) {
    gSaveBuffer.menuData.wideMode = mode;

    gMainMenuDataModified = TRUE;
    save_main_menu_data();
}
#endif

u32 save_file_get_sound_mode(void) {
    if (gSaveBuffer.menuData.soundMode >= SOUND_MODE_COUNT) {
        return 0;
    }

    return gSaveBuffer.menuData.soundMode;
}

void save_file_move_cap_to_default_location(void) {
    if (save_file_get_flags() & SAVE_FLAG_CAP_ON_GROUND) {
        switch (gSaveBuffer.files[gCurrSaveFileNum - 1][0].capLevel) {
            case LEVEL_SSL:
                save_file_set_flags(SAVE_FLAG_CAP_ON_KLEPTO);
                break;
            case LEVEL_SL:
                save_file_set_flags(SAVE_FLAG_CAP_ON_MR_BLIZZARD);
                break;
            case LEVEL_TTM:
                save_file_set_flags(SAVE_FLAG_CAP_ON_UKIKI);
                break;
        }
        save_file_clear_flags(SAVE_FLAG_CAP_ON_GROUND);
    }
}

#if MULTILANG
void eu_set_language(u16 language) {
    gSaveBuffer.menuData.language = language;
    gMainMenuDataModified = TRUE;
    save_main_menu_data();
}

u32 eu_get_language(void) {
    return gSaveBuffer.menuData.language;
}
#endif

void disable_warp_checkpoint(void) {
    // check_warp_checkpoint() checks to see if gWarpCheckpoint.courseNum != COURSE_NONE
    gWarpCheckpoint.courseNum = COURSE_NONE;
}

/**
 * Checks the upper bit of the WarpNode->destLevel byte to see if the
 * game should set a warp checkpoint.
 */
void check_if_should_set_warp_checkpoint(struct WarpNode *warpNode) {
    if (warpNode->destLevel & WARP_CHECKPOINT) {
        // Overwrite the warp checkpoint variables.
        gWarpCheckpoint.actNum = gCurrActNum;
        gWarpCheckpoint.courseNum = gCurrCourseNum;
        gWarpCheckpoint.levelID = warpNode->destLevel & 0x7F;
        gWarpCheckpoint.areaNum = warpNode->destArea;
        gWarpCheckpoint.warpNode = warpNode->destNode;
    }
}

/**
 * Checks to see if a checkpoint is properly active or not. This will
 * also update the level, area, and destination node of the input WarpNode.
 * returns TRUE if input WarpNode was updated, and FALSE if not.
 */
s32 check_warp_checkpoint(struct WarpNode *warpNode) {
    s16 warpCheckpointActive = FALSE;
    s16 currCourseNum = gLevelToCourseNumTable[(warpNode->destLevel & 0x7F) - 1];

    // gSavedCourseNum is only used in this function.
    if (gWarpCheckpoint.courseNum != COURSE_NONE && gSavedCourseNum == currCourseNum
        && gWarpCheckpoint.actNum == gCurrActNum) {
        warpNode->destLevel = gWarpCheckpoint.levelID;
        warpNode->destArea = gWarpCheckpoint.areaNum;
        warpNode->destNode = gWarpCheckpoint.warpNode;
        warpCheckpointActive = TRUE;
    } else {
        // Disable the warp checkpoint just in case the other 2 conditions failed?
        gWarpCheckpoint.courseNum = COURSE_NONE;
    }

    return warpCheckpointActive;
}
