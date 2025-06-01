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

void setPlayerSpeed(float playerSpeedMultiplier)
{
    extern int32_t originalSpeed;

    if (playerSpeedMultiplier < 0.1f || playerSpeedMultiplier > 5.0f)
        return;

    uintptr_t playerPtr = GetPointer(baseAddress, offsets::CPlayer);
    if (!playerPtr || playerPtr < 0x10000)
        return;

    CPlayer* player = reinterpret_cast<CPlayer*>(playerPtr);

    if (originalSpeed == 0)
        originalSpeed = player->PlayerSpeed;

    player->PlayerSpeed = static_cast<int32_t>(originalSpeed * playerSpeedMultiplier);
}