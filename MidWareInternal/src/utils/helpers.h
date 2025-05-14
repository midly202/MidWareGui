#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "kiero.h"
#include <chrono>
#include <cstdlib>
#include <d3d11.h>
#include <dxgi.h>
#include <stdexcept>
#include <string>
#include <thread>
#include <Windows.h>

std::string WideToNarrow(const std::wstring& wstr);

uintptr_t GetPointer(uintptr_t base, const std::vector<uintptr_t>& offsets);