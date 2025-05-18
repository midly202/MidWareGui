#include "functions.h"

int APIENTRY WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    // ------------------------------------------------------------------------- \\
    // --------------------------------Injection-------------------------------- \\
    // ------------------------------------------------------------------------- \\ 

    // std::string dllPath = GetDLLPath();
    std::string dllPath = GetCurrentDirectoryPath() + "\\MidWare.dll";
    if (dllPath.empty())
    {
        return 1;
    }
    std::string procName = "RainbowSix.exe";
    uintptr_t procId = GetProcId(procName);

    if (!InjectDLL(procId, dllPath))
    {
        std::cerr << "[!] Injection failed.\n";
        return 1;
    }

    std::string pythonScriptPath = GetCurrentDirectoryPath() + "\\ocr.py";
    PROCESS_INFORMATION pythonProc = StartPythonScript(pythonScriptPath);

    // ------------------------------------------------------------------------- \\
    // -----------------------------Allocate memory----------------------------- \\
    // ------------------------------------------------------------------------- \\ 

    struct InternalFlags
    {
        bool glowESP;
        bool runShoot;
        bool boltScript;
        bool infGadget;
        bool goOutside;
        bool exit;
        uintptr_t runShootAddy1;
        uintptr_t runShootAddy2;
        char weaponName[64];
    };

    HANDLE hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
        sizeof(InternalFlags), "MySharedMemory");

    if (!hMapFile)
    {
        std::cerr << "Could not create file mapping object. Error: " << GetLastError() << "\n";
        return 1;
    }

    InternalFlags* pFlags = (InternalFlags*)MapViewOfFile(
        hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(InternalFlags));

    if (!pFlags)
    {
        std::cerr << "Could not map view of file. Error: " << GetLastError() << "\n";
        CloseHandle(hMapFile);
        return 1;
    }

    // ------------------------------------------------------------------------- \\
    // --------------------------------Set flags-------------------------------- \\
    // ------------------------------------------------------------------------- \\ 

    pFlags->glowESP = false;
    pFlags->runShoot = false;
    pFlags->boltScript = false;
    pFlags->infGadget = false;
    pFlags->goOutside = false;
    pFlags->exit = false;

    // ------------------------------------------------------------------------- \\
    // --------------------------------Read flags------------------------------- \\
    // ------------------------------------------------------------------------- \\ 

    UnlockAll();

    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, procId);
    while (!pFlags->exit)
    {
        DWORD exitCode = 0;
        if (!GetExitCodeProcess(hProc, &exitCode) || exitCode != STILL_ACTIVE)
            break;

        if (pFlags->glowESP)
        {
            GlowESP();
            pFlags->glowESP = false;
        }
        else if (pFlags->runShoot)
        {
            RunShoot(pFlags->runShootAddy1, pFlags->runShootAddy2);
            pFlags->runShoot = false;
        }
        else if (pFlags->boltScript)
        {
            BoltScript();
            pFlags->boltScript = false;
        }
		else if (pFlags->infGadget)
		{
			InfiniteSecondaryGadgets();
            pFlags->infGadget = false;
		}
		else if (pFlags->goOutside)
		{
			GoOutside();
			pFlags->goOutside = false;
		}

        Sleep(10);
    }

    // ------------------------------------------------------------------------- \\
    // --------------------------Free allocated memory-------------------------- \\
    // ------------------------------------------------------------------------- \\ 

    StopPythonScript(pythonProc);
    UnmapViewOfFile(pFlags);
    CloseHandle(hMapFile);
    return 0;
}
