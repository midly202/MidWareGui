#include "features/weapon.h"
#include "hooks/hooks.h"
#include "utils/helpers.h"

bool shouldUnload = false;
HMODULE g_hModule = NULL;
uintptr_t baseAddress = (uintptr_t)GetModuleHandleA("RainbowSix.exe");

std::wstring currentWeaponName = L"";

DWORD WINAPI CheatThread(LPVOID)
{
    while (!shouldUnload)
    {
        try {
            auto result = readWeaponData();
            currentWeaponName = result;
        }
        catch (...) {
            OutputDebugStringA("Caught C++ exception in readWeaponData\n");
        }
        Sleep(10);
    }
    return 0;
    return 0;
}

DWORD WINAPI MonitorKeys(LPVOID)
{
    while (!GetAsyncKeyState(VK_INSERT))
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    shouldUnload = true;

    // Wait for Present hook to finish and exit
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Cleanup();
    FreeLibraryAndExitThread(g_hModule, 0);
    return 0;
}

DWORD WINAPI SetupHook(LPVOID instance)
{
    while (kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    kiero::bind(8, (void**)&oPresent, hkPresent);

    CreateThread(nullptr, 0, CheatThread, nullptr, 0, nullptr);
    CreateThread(nullptr, 0, MonitorKeys, nullptr, 0, nullptr);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        g_hModule = hModule;
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)SetupHook, nullptr, 0, nullptr);
        break;
    }

    return TRUE;
}