#pragma once
#include "gameCode.h"

/// @brief Free space for the arm9 patches of the DS Download Play app.
///        Download Play keeps the child boot information at 0x023FE800-0x023FF000
///        (cart header copy at 0x023FE940, child boot stub at 0x023FEE00) and clears
///        main memory below it before starting the child, so patches must live in
///        the unused part between the cart header copy and the boot stub.
#define DOWNLOAD_PLAY_ARM9_PATCH_SPACE          0x023FEAA0
#define DOWNLOAD_PLAY_ARM9_PATCH_SPACE_SIZE     0x300

/// @brief Free space for the arm7 patches of the DS Download Play app, directly after the arm9 patch space.
#define DOWNLOAD_PLAY_ARM7_PATCH_SPACE          (DOWNLOAD_PLAY_ARM9_PATCH_SPACE + DOWNLOAD_PLAY_ARM9_PATCH_SPACE_SIZE)
#define DOWNLOAD_PLAY_ARM7_PATCH_SPACE_SIZE     0x60

/// @brief Returns whether the given game code belongs to the DS Download Play app (HNDx),
///        which has no card or save functions to patch.
static inline bool IsDownloadPlayApp(u32 gameCode)
{
    return (gameCode & 0xFFFFFF) == GAMECODE_NO_REGION("HND");
}
