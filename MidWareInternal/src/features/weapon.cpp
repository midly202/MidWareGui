#include "../utils/helpers.h"
#include "weapon.h"


CCurrentWeapon6* currentWeapon6;
extern uintptr_t baseAddress;
extern bool shouldExit;

std::wstring readWeaponData()
{
    uintptr_t weaponPtr = GetPointer(baseAddress, offsets::CCurrentWeapon6);
    if (!weaponPtr || weaponPtr < 0x10000)
        return L"";  // Invalid address

    CCurrentWeapon6* weapon = reinterpret_cast<CCurrentWeapon6*>(weaponPtr);
    if (!weapon)
        return L"";

    static thread_local wchar_t safeBuffer[64] = { 0 };

    // Defensive memory copy
    memcpy(safeBuffer, weapon->currWeaponName, sizeof(safeBuffer) - sizeof(wchar_t));
    safeBuffer[(sizeof(safeBuffer) / sizeof(wchar_t)) - 1] = L'\0';

    return std::wstring(safeBuffer);
}

void infiniteAmmo(const std::wstring& weaponName)
{
    if (weaponName.empty()) return;

    uintptr_t weaponPtr = GetPointer(baseAddress, offsets::WeaponComponent);
    if (!weaponPtr || weaponPtr < 0x10000)
        return;

    WeaponComponent* weaponComponent = reinterpret_cast<WeaponComponent*>(weaponPtr);

    weaponComponent->gunAmmo = 10000;
}

void restoreAmmo(const std::wstring& weaponName)
{
    if (weaponName.empty()) return;

    uintptr_t weaponPtr = GetPointer(baseAddress, offsets::WeaponComponent);
    if (!weaponPtr || weaponPtr < 0x10000)
        return;

    WeaponComponent* weaponComponent = reinterpret_cast<WeaponComponent*>(weaponPtr);
    if (weaponComponent->gunAmmo > 9000)
        weaponComponent->gunAmmo = weaponSettingsMap[weaponName].originalAmmoValue;
}
