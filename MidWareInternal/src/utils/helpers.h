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

extern std::unordered_map<std::wstring, WeaponSettings> weaponSettingsMap;

std::string WideToNarrow(const std::wstring& wstr);

uintptr_t GetPointer(uintptr_t base, const std::vector<uintptr_t>& offsets);