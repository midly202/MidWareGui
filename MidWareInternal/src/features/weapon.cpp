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

void knifeReach()
{
    uintptr_t knifePtr = GetPointer(baseAddress, offsets::Knife);
    if (!knifePtr || knifePtr < 0x10000)
        return;

    Knife* knife = reinterpret_cast<Knife*>(knifePtr);

    if (knife->knifeDistancePlayers >= 1.2f && knife->knifeDistanceWalls >= 1.2f)
    {
        knife->knifeDistancePlayers = 9999.0f;
        knife->knifeDistanceWalls = 9999.0f;
    }
}

void restoreKnifeReach()
{
    uintptr_t knifePtr = GetPointer(baseAddress, offsets::Knife);
    if (!knifePtr || knifePtr < 0x10000)
        return;

    Knife* knife = reinterpret_cast<Knife*>(knifePtr);

    if (knife->knifeDistancePlayers == 9999.0f)
    {
        knife->knifeDistancePlayers = 1.299999952f;
        knife->knifeDistanceWalls = 1.299999952f;
    }
}

void noSpread(const std::wstring& weaponName, int percentage)
{
    if (weaponName.empty() || percentage < 0 || percentage > 100) return;

    uintptr_t weaponPtr = GetPointer(baseAddress, offsets::CCurrentWeapon5);
    if (!weaponPtr || weaponPtr < 0x10000)
        return;

    CCurrentWeapon5* currentWeapon = reinterpret_cast<CCurrentWeapon5*>(weaponPtr);

    float originalSpread = weaponSettingsMap[weaponName].originalSpreadValue;

    if (percentage == 0)
        currentWeapon->GunSpread = originalSpread;

    else if (percentage == 100)
        currentWeapon->GunSpread = 0;

    else
        currentWeapon->GunSpread = originalSpread * (1.0f - (percentage / 100.0f));
}

void restoreSpread(const std::wstring& weaponName)
{
    if (weaponName.empty()) return;

    uintptr_t weaponPtr = GetPointer(baseAddress, offsets::CCurrentWeapon5);
    if (!weaponPtr || weaponPtr < 0x10000)
        return;

    CCurrentWeapon5* currentWeapon = reinterpret_cast<CCurrentWeapon5*>(weaponPtr);

    currentWeapon->GunSpread = weaponSettingsMap[weaponName].originalSpreadValue;
}

void caliberSelect(const std::wstring& weaponName, int caliber)
{
    if (weaponName.empty() || caliber < 0) return;

    uintptr_t caliberPtr = GetPointer(baseAddress, offsets::CCurrentWeaponCaliber);
    if (!caliberPtr || caliberPtr < 0x10000)
        return;

    CCurrentWeaponCaliber* gunCaliber = reinterpret_cast<CCurrentWeaponCaliber*>(caliberPtr);

    // Map the caliber index to the corresponding caliber ID
    uint32_t caliberID = 0; // Default to an invalid ID
    switch (caliber) {
    case 0: caliberID = 224916230; break; // "Bullet_SuperLow"
    case 1: caliberID = 1635385480; break; // "Bullet_Low"
    case 2: caliberID = 3864566559; break; // "Bullet_Mid"
    case 3: caliberID = 156726242; break; // "Bullet_High"
    case 4: caliberID = 54247478; break; // "Bullet_SuperHigh"
    case 5: caliberID = 3321364808; break; // "Bullet_MaxPower"
    case 6: caliberID = 1586594484; break; // "Bullet_BOSG"
    case 7: caliberID = 3683118896; break; // "Bullet_CSRX300"
    case 8: caliberID = 2022132106; break; // "Bullet_DMR"
    case 9: caliberID = 2249646560; break; // "Bullet_DP27"
    case 10: caliberID = 1314319139; break; // "Shell_Low"
    case 11: caliberID = 3379851444; break; // "Shell_Mid"
    case 12: caliberID = 1216095593; break; // "Shell_High"
    case 13: caliberID = 1800674797; break; // "Gadget_BarrageTurret"
    case 14: caliberID = 2097060903; break; // "Gadget_BlowTorch"
    default: return; // Invalid caliber index
    }

    gunCaliber->GunCaliber = caliberID;
}