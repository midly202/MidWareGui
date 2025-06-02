#include "../utils/helpers.h"
#include "thunt.h"

extern uintptr_t baseAddress;

void lockWave()
{
    uintptr_t wavePtr = GetPointer(baseAddress, offsets::THuntWave);
    if (!wavePtr || wavePtr < 0x10000)
        return;

    CThuntWave* thuntWave = reinterpret_cast<CThuntWave*>(wavePtr);

    thuntWave->CurrentWave = 0;
}

void gameState(int state)
{
    uintptr_t roundPtr = GetPointer(baseAddress, offsets::THuntWave);
    if (!roundPtr || roundPtr < 0x10000)
        return;

    CThuntWave* round = reinterpret_cast<CThuntWave*>(roundPtr);

    round->gameState = state;

    // Gamestate
    // 0 = Invalid                                  ENDS MATCH
    // 1 = Shutdown                                 ENDS MATCH              
    // 2 = WaitingPlaylistCategorySelection         ENDS MATCH
    // 3 = WaitingForMatchStart                     ENDS ROUND (loss)
    // 4 = PendingRoundSetup                        ENDS ROUND (loss)
    // 5 = WaitingReadyToPlay                       ENDS ROUND (loss)
    // 6 = Playing                                  N/A
    // 7 = PlayerControlAllowed                     N/A
}

void infiniteTime(bool infTime)
{
    uintptr_t roundPtr = GetPointer(baseAddress, offsets::THuntWave);
    if (!roundPtr || roundPtr < 0x10000 || !roundPtr || roundPtr < 0x10000)
        return;

    CThuntWave* infiniteTime = reinterpret_cast<CThuntWave*>(roundPtr);

    if (infTime && infiniteTime->gameState == 7)
        infiniteTime->infTime = 286331153;
    else
        infiniteTime->infTime = 912068501;
}