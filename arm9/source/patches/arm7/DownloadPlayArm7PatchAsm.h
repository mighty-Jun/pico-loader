#pragma once
#include "../PatchCode.h"
#include "sections.h"

DEFINE_SECTION_SYMBOLS(patch_downloadplay7);

extern "C" void patch_downloadplay7_entry(void);

/// @brief Arm7 side of the reboot into Pico Loader for DS Download Play: waits in the arm7 wram
///        until the arm9 loaded Pico Loader, then enters it.
class DownloadPlayArm7PatchCode : public PatchCode
{
public:
    explicit DownloadPlayArm7PatchCode(PatchHeap& patchHeap)
        : PatchCode(SECTION_START(patch_downloadplay7), SECTION_SIZE(patch_downloadplay7), patchHeap) { }

    const void* GetBoot7Function() const
    {
        return GetAddressAtTarget((void*)patch_downloadplay7_entry);
    }
};
