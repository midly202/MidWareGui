#pragma once
#include <string>
#include <Windows.h>

std::wstring readWeaponData();

void infiniteAmmo(const std::wstring& weaponName);

void restoreAmmo(const std::wstring& weaponName);

void noRecoil(const std::wstring& weaponName);

void restoreRecoil(const std::wstring& weaponName);

void fullAuto(const std::wstring& weaponName);

void restoreFullAuto(const std::wstring& weaponName);

void rapidFire(const std::wstring& weaponName);

void restoreRapidFire(const std::wstring& weaponName);

void instaKill(const std::wstring& weaponName);

void restoreInstaKill(const std::wstring& weaponName);