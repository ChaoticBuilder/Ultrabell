#ifndef SMB_ENGINE_H
#define SMB_ENGINE_H

#include <PR/ultratypes.h>

#include "macros.h"
#include "types.h"

enum powerArray {
    SMALL,
    BIG
};

extern u8 fami_game_loop(void);

#endif // SMB_ENGINE_H