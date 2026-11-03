#pragma once
#include "gameCode.h"

/// @brief Child boot information that DS Download Play keeps for the downloaded child.
///        When the boot type is multiboot, the arm7 crt0 of the child copies the download
///        parameters (0x023FE904) and the header of the inserted card (0x023FE940) from here
///        to the shared memory.
#define DOWNLOAD_PLAY_BOOT_INFO                 0x023FE800
#define DOWNLOAD_PLAY_BOOT_INFO_SIZE            0x800

/// @brief Header of the inserted card for the downloaded child (0x160 bytes).
#define DOWNLOAD_PLAY_CARD_ROM_HEADER           0x023FE940

/// @brief Free space for the arm9 patches of the DS Download Play app.
///        Download Play keeps the child boot information (cart header copy at 0x023FE940,
///        child boot stub at 0x023FEE00) and clears main memory below it before starting
///        the child, so patches must live in the unused part between the cart header copy
///        and the boot stub.
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
