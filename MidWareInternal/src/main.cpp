#include "features/weapon.h"
#include "hooks/hooks.h"
#include "utils/helpers.h"

bool shouldUnload = false;
HMODULE g_hModule = NULL;
uintptr_t baseAddress = (uintptr_t)GetModuleHandleA("RainbowSix.exe");

std::wstring weaponName = L"";
std::wstring lastWeaponName = L"";
std::wstring fallbackName = L"";

std::unordered_map<std::wstring, WeaponSettings> weaponSettingsMap;

HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "MySharedMemory");
InternalFlags* pFlags = (InternalFlags*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(InternalFlags));

DWORD WINAPI CheatThread(LPVOID)
{
    while (!shouldUnload)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        auto result = readWeaponData();
        weaponName = result;

        if (weaponName.empty())
        {
            std::string ocrValue = std::string(pFlags->weaponName, strnlen(pFlags->weaponName, 64));
            if (!ocrValue.empty())
                weaponName = NarrowToWide(ocrValue);
        }

        std::wstring newWeaponName = !weaponName.empty() ? weaponName : fallbackName;

        if (!newWeaponName.empty() && newWeaponName != weaponName)
        {
            weaponName = newWeaponName;
            lastWeaponName = weaponName;

            // Create new weapon entry if not already present
            if (weaponSettingsMap.find(weaponName) == weaponSettingsMap.end())
            {
                uintptr_t weaponPtr = GetPointer(baseAddress, offsets::WeaponComponent);
                if (!weaponPtr || weaponPtr < 0x10000)
                    return 1;

                uintptr_t caliberPtr = GetPointer(baseAddress, offsets::CCurrentWeaponCaliber);
                if (!caliberPtr || caliberPtr < 0x10000)
                    return 1;

                WeaponComponent* weaponComponent = reinterpret_cast<WeaponComponent*>(weaponPtr);
                CCurrentWeaponCaliber* gunCaliber = reinterpret_cast<CCurrentWeaponCaliber*>(caliberPtr);

                weaponSettingsMap[weaponName] = WeaponSettings();

                WeaponSettings& settings = weaponSettingsMap[weaponName];
                settings.originalAmmoValue = weaponComponent->gunAmmo;
                settings.caliberIndex = MapCaliberIDToIndex(gunCaliber->GunCaliber);
            }
        }

        const auto& settings = weaponSettingsMap[weaponName];

        if (!weaponName.empty())
        {
            if (settings.infiniteAmmo)
                infiniteAmmo(weaponName);
            else
                restoreAmmo(weaponName);
        }

        if (weaponName.empty())
        {
            weaponSettingsMap[weaponName].infiniteAmmo = false;
        }
    }
    return 0;
}


DWORD WINAPI MonitorKeys(LPVOID)
{
    while (!GetAsyncKeyState(VK_INSERT))
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    shouldUnload = true;

    // Wait for Present hook to finish and exit
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    pFlags->exit = true;
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