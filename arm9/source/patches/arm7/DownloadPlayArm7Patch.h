#pragma once
#include "../Patch.h"

/// @brief Arm7 patch to make DS Download Play reboot into Pico Loader (multiboot)
///        instead of jumping to the downloaded child.
class DownloadPlayArm7Patch : public Patch
{
public:
    bool FindPatchTarget(PatchContext& patchContext) override;
    void ApplyPatch(PatchContext& patchContext) override;

private:
    u32* _childJump = nullptr;
};
