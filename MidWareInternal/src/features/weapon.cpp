#include "../utils/helpers.h"
#include "../utils/offsets.h"
#include "weapon.h"


CCurrentWeapon6* currentWeapon6;
extern uintptr_t baseAddress;
extern bool shouldExit;

const wchar_t* TryGetWeaponNameRaw()
{
    __try
    {
        uintptr_t weaponPtr = GetPointer(baseAddress, offsets::CCurrentWeapon6);
        CCurrentWeapon6* weapon = reinterpret_cast<CCurrentWeapon6*>(weaponPtr);
        if (!weapon)
            return nullptr;

        static thread_local wchar_t safeBuffer[64] = { 0 };

        // Instead of wcsncpy_s, we copy raw memory and manually null-terminate
        memcpy(safeBuffer, weapon->currWeaponName, sizeof(safeBuffer) - sizeof(wchar_t));
        safeBuffer[(sizeof(safeBuffer) / sizeof(wchar_t)) - 1] = L'\0';

        return safeBuffer;
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        return nullptr;
    }
}

std::wstring readWeaponData()
{
    const wchar_t* name = TryGetWeaponNameRaw();
    return name ? std::wstring(name) : L"";
}