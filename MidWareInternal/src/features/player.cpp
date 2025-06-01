#include "../utils/helpers.h"
#include "player.h"

extern uintptr_t baseAddress;

void noClip()
{
    uintptr_t noClipPtr = GetPointer(baseAddress, offsets::NoClip);
    if (!noClipPtr || noClipPtr < 0x10000)
        return;

    CNoClip* noClip = reinterpret_cast<CNoClip*>(noClipPtr);

    noClip->NoClip = -1;
}

void restoreNoClip()
{
    uintptr_t noClipPtr = GetPointer(baseAddress, offsets::NoClip);
    if (!noClipPtr || noClipPtr < 0x10000)
        return;

    CNoClip* noClip = reinterpret_cast<CNoClip*>(noClipPtr);

    noClip->NoClip = 0.0f;
}