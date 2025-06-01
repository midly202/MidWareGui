#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "kiero.h"
#include "structs.h"
#include "offsets.h"
#include <chrono>
#include <cstdlib>
#include <d3d11.h>
#include <dxgi.h>
#include <stdexcept>
#include <string>
#include <thread>
#include <Windows.h>
#include <unordered_map>
#include <iostream>
#include <cstdint>
#include <TlHelp32.h>
#include <winternl.h>
#include <Psapi.h>

extern std::unordered_map<std::wstring, WeaponSettings> weaponSettingsMap;

std::string WideToNarrow(const std::wstring& wstr);

std::wstring NarrowToWide(const std::string& str);

uintptr_t GetPointer(uintptr_t base, const std::vector<uintptr_t>& offsets);

MODULEINFO GetModuleInfo(const char* szModule);

uintptr_t FindPattern(const char* module, const char* pattern, const char* mask);

int MapCaliberIDToIndex(uint32_t caliberID);

float getPlayerFOV();

float getGunFOV();