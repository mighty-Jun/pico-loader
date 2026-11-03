#include "common.h"
#include "../PatchContext.h"
#include "PokemonDownloaderArm7PatchAsm.h"
#include "DownloadPlayArm7Patch.h"

// End of the arm7 child boot function (copied to 0x0380F62C before the child starts):
//   ldr r3,= 0x03000000
//   ldr r12, [r3, #-0x1CC] // child arm7 entry point from the header at 0x02FFFE00
//   mov lr, r12
//   mov r11, #0x04000000
// By then the child arm7 has been copied to its load address.
static const u32 sChildJumpPattern[] = { 0xE59F301Cu, 0xE513C1CCu, 0xE1A0E00Cu, 0xE3A0B301u };

#define CHILD_JUMP_LITERAL_OFFSET   (8 + 0x1C)
#define CHILD_JUMP_LOAD_OFFSET      0x1CC

bool DownloadPlayArm7Patch::FindPatchTarget(PatchContext& patchContext)
{
    _childJump = patchContext.FindPattern32(sChildJumpPattern, sizeof(sChildJumpPattern));
    return _childJump != nullptr;
}

void DownloadPlayArm7Patch::ApplyPatch(PatchContext& patchContext)
{
    if (!_childJump)
    {
        return;
    }

    // Same handshake as for the Pokemon downloader: wait for the arm9 to load Pico Loader, then enter it.
    auto patchCode = patchContext.GetPatchCodeCollection().AddUniquePatchCode<PokemonDownloaderArm7PatchCode>(
        patchContext.GetPatchHeap());

    // Let the boot function load the reboot code address instead of the child entry point.
    auto rebootAddress = (u32*)patchContext.GetPatchHeap().Alloc(sizeof(u32));
    *rebootAddress = (u32)patchCode->GetBoot7Function();
    u32* literal = (u32*)((u32)_childJump + CHILD_JUMP_LITERAL_OFFSET);
    *literal = (u32)rebootAddress + CHILD_JUMP_LOAD_OFFSET;
    LOG_DEBUG("Download Play child arm7 boot redirected to 0x%x\n", *rebootAddress);
}
