#pragma once
#include <string>
#include <Windows.h>

std::wstring readWeaponData();

void infiniteAmmo(const std::wstring& weaponName);

void restoreAmmo(const std::wstring& weaponName);