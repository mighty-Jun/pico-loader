#pragma once
#include "../Patch.h"
#include "LoaderInfo.h"

/// @brief Arm9 patch to make DS Download Play reboot into Pico Loader (multiboot)
///        instead of jumping to the downloaded child.
class DownloadPlayArm9Patch : public Patch
{
public:
    explicit DownloadPlayArm9Patch(const loader_info_t* loaderInfo)
        : _loaderInfo(loaderInfo) { }

    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

private:
    u32* _childJump = nullptr;
    const loader_info_t* _loaderInfo;
};
