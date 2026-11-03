#include "common.h"
#include "../PatchContext.h"
#include "../platform/LoaderPlatform.h"
#include "PokemonDownloaderArm9PatchCode.h"
#include "DownloadPlayArm9Patch.h"

// End of the arm9 child boot stub (copied to 0x023FEE00 before the child starts):
//   ldr r3,= 0x03000000
//   ldr r12, [r3, #-0x1DC] // child arm9 entry point from the header at 0x02FFFE00
//   mov lr, r12
//   ldr r11,= 0x03000000
// At this point Download Play has put the child in place and cleaned up, and both cpus
// jump to the child after a final IPCSYNC handshake.
static const u32 sChildJumpPattern[] = { 0xE59F301Cu, 0xE513C1DCu, 0xE1A0E00Cu, 0xE59FB014u };

#define CHILD_JUMP_LITERAL_OFFSET   (8 + 0x1C)
#define CHILD_JUMP_LOAD_OFFSET      0x1DC

bool DownloadPlayArm9Patch::FindPatchTarget(PatchContext& patchContext)
{
    _childJump = patchContext.FindPattern32(sChildJumpPattern, sizeof(sChildJumpPattern));
    return _childJump != nullptr;
}

void DownloadPlayArm9Patch::ApplyPatch(PatchContext& patchContext)
{
    if (!_childJump)
    {
        return;
    }

    auto loaderInfoTarget = (loader_info_t*)patchContext.GetPatchHeap().Alloc(sizeof(loader_info_t));
    memcpy(loaderInfoTarget, _loaderInfo, sizeof(loader_info_t));

    auto sdReadPatchCode = patchContext.GetLoaderPlatform()->CreateSdReadPatchCode(
        patchContext.GetPatchCodeCollection(), patchContext.GetPatchHeap());
    // Same reboot as for the Pokemon downloader: load Pico Loader into vram,
    // set the multiboot flag and hand over to it together with the arm7.
    auto patchCode = patchContext.GetPatchCodeCollection().AddUniquePatchCode<PokemonDownloaderArm9PatchCode>
    (
        patchContext.GetPatchHeap(),
        loaderInfoTarget,
        sdReadPatchCode
    );

    // Let the stub load the reboot code address instead of the child entry point.
    auto rebootAddress = (u32*)patchContext.GetPatchHeap().Alloc(sizeof(u32));
    *rebootAddress = (u32)patchCode->GetBoot9Function();
    u32* literal = (u32*)((u32)_childJump + CHILD_JUMP_LITERAL_OFFSET);
    *literal = (u32)rebootAddress + CHILD_JUMP_LOAD_OFFSET;
    LOG_DEBUG("Download Play child boot redirected to 0x%x\n", *rebootAddress);
}
