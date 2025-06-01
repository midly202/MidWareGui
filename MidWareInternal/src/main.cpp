#include "features/weapon.h"
#include "features/player.h"
#include "features/visual.h"
#include "hooks/hooks.h"
#include "utils/helpers.h"

bool shouldUnload = false;
HMODULE g_hModule = NULL;
uintptr_t baseAddress = (uintptr_t)GetModuleHandleA("RainbowSix.exe");

HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "MySharedMemory");
InternalFlags* pFlags = (InternalFlags*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(InternalFlags));

std::unordered_map<std::wstring, WeaponSettings> weaponSettingsMap;

CheatSettings cheatSettings;

std::wstring weaponName = L"";
std::wstring lastWeaponName = L"";
std::wstring displayWeaponName = L"";

int32_t originalSpeed = 0;

DWORD WINAPI CheatThread(LPVOID)
{
    /*
    AllocConsole();
    FILE* f = nullptr;
    freopen_s(&f, "CONOUT$", "w", stdout);
    system("cls");
    SetConsoleTitleA("MidWare Internal - Console");
    */
    cheatSettings.playerFOV = getPlayerFOV();
    cheatSettings.gunFOV = getGunFOV();

    while (!shouldUnload)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        // get weapon name from dev string
        auto result = readWeaponData();
        weaponName = result;

        // if no dev string found, get weapon name from OCR
        if (weaponName.empty())
        {
            std::string ocrValue = std::string(pFlags->weaponName, strnlen(pFlags->weaponName, 64));
            if (!ocrValue.empty())
                weaponName = NarrowToWide(ocrValue);
        }

        if (!weaponName.empty())
        {
            displayWeaponName = weaponName;
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

                uintptr_t currWeapon2Ptr = GetPointer(baseAddress, offsets::CCurrentWeapon2);
                if (!currWeapon2Ptr || currWeapon2Ptr < 0x10000)
                    return 1;

                uintptr_t currWeapon3Ptr = GetPointer(baseAddress, offsets::CCurrentWeapon3);
                if (!currWeapon3Ptr || currWeapon3Ptr < 0x10000)
                    return 1;

                uintptr_t currWeapon4Ptr = GetPointer(baseAddress, offsets::CCurrentWeapon4);
                if (!currWeapon4Ptr || currWeapon4Ptr < 0x10000)
                    return 1;

                uintptr_t currWeapon5Ptr = GetPointer(baseAddress, offsets::CCurrentWeapon5);
                if (!currWeapon5Ptr || currWeapon5Ptr < 0x10000)
                    return 1;

                WeaponComponent* weaponComponent = reinterpret_cast<WeaponComponent*>(weaponPtr);
                CCurrentWeaponCaliber* gunCaliber = reinterpret_cast<CCurrentWeaponCaliber*>(caliberPtr);
                CCurrentWeapon2* currWeapon2 = reinterpret_cast<CCurrentWeapon2*>(currWeapon2Ptr);
                CCurrentWeapon3* currWeapon3 = reinterpret_cast<CCurrentWeapon3*>(currWeapon3Ptr);
                CCurrentWeapon4* currWeapon4 = reinterpret_cast<CCurrentWeapon4*>(currWeapon4Ptr);
                CCurrentWeapon5* currWeapon5 = reinterpret_cast<CCurrentWeapon5*>(currWeapon5Ptr);

                weaponSettingsMap[weaponName] = WeaponSettings();

                WeaponSettings& settings = weaponSettingsMap[weaponName];
                settings.originalAmmoValue = weaponComponent->gunAmmo;
                settings.originalRecoilValue = currWeapon2->pGunRecoil;
                settings.originalFiremodeValue = weaponComponent->gunFireMode;
                settings.originalFirerateValue = currWeapon3->GunFireRate;
                settings.originalDamageValue = currWeapon4->GunDamage;
				settings.originalSpreadValue = currWeapon5->GunSpread;
                settings.caliberIndex = MapCaliberIDToIndex(gunCaliber->GunCaliber);
            }
        }

		// so menu doesn't show last OCR weapon name
        else if (weaponName.empty())
            displayWeaponName = L"";

        const auto& settings = weaponSettingsMap[weaponName];

        if (!weaponName.empty())
        {
            if (settings.infiniteAmmo)
                infiniteAmmo(weaponName);
            else
                restoreAmmo(weaponName);

            if (settings.noRecoil)
                noRecoil(weaponName);
            else
                restoreRecoil(weaponName);

            if (settings.fullAuto)
                fullAuto(weaponName);
            else
                restoreFullAuto(weaponName);

            if (settings.rapidFire)
                rapidFire(weaponName);
            else
                restoreRapidFire(weaponName);

            if (settings.instaKill)
                instaKill(weaponName);
            else
                restoreInstaKill(weaponName);

            if (settings.spreadReduction)
                noSpread(weaponName, settings.spreadReduction);
            else
                restoreSpread(weaponName);

            if (settings.caliberIndex != 15)
                caliberSelect(weaponName, settings.caliberIndex);
        }

        else
        {
            weaponSettingsMap[weaponName].infiniteAmmo = false;
            weaponSettingsMap[weaponName].fullAuto = false;
            weaponSettingsMap[weaponName].rapidFire = false;
            weaponSettingsMap[weaponName].instaKill = false;
            weaponSettingsMap[weaponName].noRecoil = false;
            weaponSettingsMap[weaponName].spreadReduction = 0;
            weaponSettingsMap[weaponName].caliberIndex = 15;
        }

        if (cheatSettings.toggleRunShoot)
        {
            pFlags->runShoot = true;
            cheatSettings.toggleRunShoot = false;
        }

        if (cheatSettings.toggleGlowESP)
        {
            pFlags->glowESP = true;
            cheatSettings.toggleGlowESP = false;
        }

        if (cheatSettings.toggleBoltScript)
        {
            pFlags->boltScript = true;
            cheatSettings.toggleBoltScript = false;
        }

        if (cheatSettings.toggleGoOutside)
        {
            pFlags->goOutside = true;
            cheatSettings.toggleGoOutside = false;
        }

        if (cheatSettings.toggleInfGadgets)
        {
            pFlags->infGadget = true;
            cheatSettings.toggleInfGadgets = false;
        }

        /*
        If health is 0 or invalid, disable knife reach (write the default values). 
        This is required as leaving knife distance on a non-default value will cause the game to crash when switching sessions.
		Health is used as it's the first memory to be unloaded when switching sessions.
        */
        int32_t originalHealth = 100;
        uintptr_t healthPtr = GetPointer(baseAddress, offsets::GodMode);
        if (healthPtr && healthPtr > 0x10000)
        {
            Health* health = reinterpret_cast<Health*>(healthPtr);
            if (health->Health < 1000)
			    originalHealth = health->Health;

            if (!health->Health)
                cheatSettings.knifeReach = false;

            if (cheatSettings.godMode)
                health->Health = 9999;
            else
				health->Health = originalHealth;

            if (cheatSettings.noClip)
                noClip();
            else
                restoreNoClip();

            if (cheatSettings.knifeReach && health->Health) // set default values if health is zero
                knifeReach();
            else
                restoreKnifeReach();

            if (cheatSettings.removeSky)
                removeSky();
            else
                restoreSky();

			setPlayerSpeed(cheatSettings.playerSpeed);
            setPlayerFOV(cheatSettings.playerFOV);
            setGunFOV(cheatSettings.gunFOV);

            setCamFOV(cheatSettings.camFOV);
            setCamClippingPlane(cheatSettings.camDistance);

            setCamX(cheatSettings.camX);
            setCamY(cheatSettings.camY);
            setCamZ(cheatSettings.camZ);

            setCamPitch(cheatSettings.camPitch);
            setCamRoll(cheatSettings.camRoll);
            setCamYaw(cheatSettings.camYaw);
        }
        else
        {
            restoreKnifeReach(); // set default values if health is invalid
            originalSpeed = 0;
        }
    }

    /*
    if (f) fclose(f);
    FreeConsole();
    */
    return 0;
}

DWORD WINAPI MonitorKeys(LPVOID)
{
    uintptr_t address1 = FindPattern("RainbowSix.exe", "\x80\xB9\x00\x00\x00\x00\x00\x74\x15\xE8", "xx?????xxx");
    uintptr_t address2 = FindPattern("RainbowSix.exe", "\x80\xB9\x00\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\x48\x89\xF1\x31", "xx?????xx????xxxx");

    while (!address1 && !address2)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

    pFlags->runShootAddy1 = address1;
    pFlags->runShootAddy2 = address2;

    while (!GetAsyncKeyState(VK_INSERT))
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    shouldUnload = true;

    // Wait for Present hook to finish and exit
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    pFlags->exit = true;
    if (pFlags) 
    {
        UnmapViewOfFile(pFlags);
        pFlags = nullptr;
    }
    if (hMapFile) 
    {
        CloseHandle(hMapFile);
        hMapFile = nullptr;
    }
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