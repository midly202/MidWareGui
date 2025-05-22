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

void noRecoil(const std::wstring& weaponName)
{
    if (weaponName.empty()) return;

    uintptr_t weaponPtr = GetPointer(baseAddress, offsets::CCurrentWeapon2);
    if (!weaponPtr || weaponPtr < 0x10000)
        return;

    CCurrentWeapon2* weapon = reinterpret_cast<CCurrentWeapon2*>(weaponPtr);

    weapon->pGunRecoil = 0;
}

void restoreRecoil(const std::wstring& weaponName)
{
    if (weaponName.empty()) return;

    uintptr_t weaponPtr = GetPointer(baseAddress, offsets::CCurrentWeapon2);
    if (!weaponPtr || weaponPtr < 0x10000)
        return;

    CCurrentWeapon2* weapon = reinterpret_cast<CCurrentWeapon2*>(weaponPtr);

    weapon->pGunRecoil = weaponSettingsMap[weaponName].originalRecoilValue;
}

void fullAuto(const std::wstring& weaponName)
{
    if (weaponName.empty()) return;

    uintptr_t firemodePtr = GetPointer(baseAddress, offsets::WeaponComponent);
    if (!firemodePtr || firemodePtr < 0x10000)
        return;

    WeaponComponent* weaponComponent = reinterpret_cast<WeaponComponent*>(firemodePtr);

    weaponComponent->gunFireMode = 0;
}

void restoreFullAuto(const std::wstring& weaponName)
{
    if (weaponName.empty()) return;

    uintptr_t firemodePtr = GetPointer(baseAddress, offsets::WeaponComponent);
    if (!firemodePtr || firemodePtr < 0x10000)
        return;

    WeaponComponent* weaponComponent = reinterpret_cast<WeaponComponent*>(firemodePtr);

    weaponComponent->gunFireMode = weaponSettingsMap[weaponName].originalFiremodeValue;
}

void rapidFire(const std::wstring& weaponName)
{
    if (weaponName.empty()) return;

    uintptr_t fireratePtr = GetPointer(baseAddress, offsets::CCurrentWeapon3);
    if (!fireratePtr || fireratePtr < 0x10000)
        return;

    CCurrentWeapon3* currentWeapon = reinterpret_cast<CCurrentWeapon3*>(fireratePtr);

    currentWeapon->GunFireRate = 9999;
}

void restoreRapidFire(const std::wstring& weaponName)
{
    if (weaponName.empty()) return;

    uintptr_t fireratePtr = GetPointer(baseAddress, offsets::CCurrentWeapon3);
    if (!fireratePtr || fireratePtr < 0x10000)
        return;

    CCurrentWeapon3* currentWeapon = reinterpret_cast<CCurrentWeapon3*>(fireratePtr);

    if (currentWeapon->GunFireRate >= 1000)
    {
        currentWeapon->GunFireRate = weaponSettingsMap[weaponName].originalFirerateValue;
    }
}

void instaKill(const std::wstring& weaponName)
{
    if (weaponName.empty()) return;

    uintptr_t weaponPtr = GetPointer(baseAddress, offsets::CCurrentWeapon4);
    if (!weaponPtr || weaponPtr < 0x10000)
        return;

    CCurrentWeapon4* currentWeapon = reinterpret_cast<CCurrentWeapon4*>(weaponPtr);

    currentWeapon->GunDamage = 9999;
}

void restoreInstaKill(const std::wstring& weaponName)
{
    if (weaponName.empty()) return;

    uintptr_t weaponPtr = GetPointer(baseAddress, offsets::CCurrentWeapon4);
    if (!weaponPtr || weaponPtr < 0x10000)
        return;

    CCurrentWeapon4* currentWeapon = reinterpret_cast<CCurrentWeapon4*>(weaponPtr);

    if (currentWeapon->GunDamage >= 8000)
    {
        currentWeapon->GunDamage = weaponSettingsMap[weaponName].originalDamageValue;
    }
}