#include "features/weapon.h"
#include "hooks/hooks.h"
#include "utils/helpers.h"

bool shouldUnload = false;
HMODULE g_hModule = NULL;
uintptr_t baseAddress = (uintptr_t)GetModuleHandleA("RainbowSix.exe");

std::wstring weaponName = L"";
std::wstring lastWeaponName = L"";

std::unordered_map<std::wstring, WeaponSettings> weaponSettingsMap;

DWORD WINAPI CheatThread(LPVOID)
{
    AllocConsole();
    FILE* f = nullptr;
    freopen_s(&f, "CONOUT$", "w", stdout);
    system("cls");
    system("mode con: cols=70 lines=42");

    HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "MySharedMemory");
    if (!hMapFile) return 1;

    InternalFlags* pFlags = (InternalFlags*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(InternalFlags));
    if (!pFlags) return 1;

    while (!shouldUnload)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        // std::string ocrValue(pFlags->weaponName);
        std::string ocrValue = std::string(pFlags->weaponName, strnlen(pFlags->weaponName, 64));
        std::string debugMessage = "[OCR] Shared memory weaponName: " + ocrValue + "\n";
        std::cout << debugMessage;

        auto result = readWeaponData();
        weaponName = result;

        if (weaponName.empty())
        {
            uintptr_t recoilPtr = GetPointer(baseAddress, offsets::CCurrentWeapon2);
            if (recoilPtr && recoilPtr > 0x10000)
            {
				CCurrentWeapon2* recoil = reinterpret_cast<CCurrentWeapon2*>(recoilPtr);
                uint64_t recoilValue = recoil->pGunRecoil;
                std::wstring fallbackName = GetWeaponNameByRecoil(recoilValue);
                if (!fallbackName.empty())
                    weaponName = fallbackName;
            }
        }


        if (!weaponName.empty() && weaponName != lastWeaponName)
        {
            lastWeaponName = weaponName;

            // If this weapon is new, create default settings
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
                
                weaponSettingsMap[weaponName] = WeaponSettings(); // all toggles false by default

                WeaponSettings& settings = weaponSettingsMap[weaponName];
                settings.originalAmmoValue = weaponComponent->gunAmmo;
                settings.caliberIndex = MapCaliberIDToIndex(gunCaliber->GunCaliber);
            }
        }

        const auto& settings = weaponSettingsMap[weaponName];

        if (settings.infiniteAmmo && weaponName != L"")
        {
            infiniteAmmo(weaponName);
        }
        else if (!settings.infiniteAmmo && weaponName != L"")
        {
            restoreAmmo(weaponName);
        }
        else
        {
            weaponSettingsMap[weaponName].infiniteAmmo = false;
        }

        // Other per-feature logic




    }
    pFlags->exit = true;
    if (f) fclose(f);
    FreeConsole();
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