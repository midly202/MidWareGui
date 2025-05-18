#pragma once

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <commdlg.h>
#include <string>
#include <Psapi.h>
#include <vector>

// ------------------------------------------------------------------------- \\
// --------------------------------Injection-------------------------------- \\
// ------------------------------------------------------------------------- \\ 

uintptr_t GetProcId(const std::string& procName)
{
    PROCESSENTRY32 entry{};
    entry.dwSize = sizeof(PROCESSENTRY32);
    uintptr_t pid = 0;  // Using uintptr_t for portability

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
    {
        std::cerr << "[!] Failed to create process snapshot.\n";
        return 0;
    }

    if (Process32First(snapshot, &entry))
    {
        do
        {
            if (_stricmp(entry.szExeFile, procName.c_str()) == 0)
            {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &entry));
    }
    CloseHandle(snapshot);
    return pid;
}

bool InjectDLL(uintptr_t pid, const std::string& dllPath)
{
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProc || hProc == INVALID_HANDLE_VALUE)
    {
        std::cerr << "[!] Failed to open target process.\n";
        return false;
    }

    void* allocAddr = VirtualAllocEx(hProc, nullptr, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!allocAddr)
    {
        std::cerr << "[!] Failed to allocate memory in target process.\n";
        CloseHandle(hProc);
        return false;
    }

    if (!WriteProcessMemory(hProc, allocAddr, dllPath.c_str(), dllPath.length() + 1, nullptr))
    {
        std::cerr << "[!] Failed to write DLL path to target process memory.\n";
        VirtualFreeEx(hProc, allocAddr, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    HANDLE hThread = CreateRemoteThread(hProc, nullptr, 0,
        reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryA),
        allocAddr, 0, nullptr);

    if (!hThread)
    {
        std::cerr << "[!] Failed to create remote thread.\n";
        VirtualFreeEx(hProc, allocAddr, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    CloseHandle(hThread);
    CloseHandle(hProc);
    return true;
}

std::string GetDLLPath()
{
    char filename[MAX_PATH] = {};
    OPENFILENAMEA ofn = { sizeof(ofn) };
    ofn.lpstrFilter = "DLL Files\0*.dll\0All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = "Select DLL to Inject";

    return GetOpenFileNameA(&ofn) ? std::string(filename) : "";
}

PROCESS_INFORMATION StartPythonScript(const std::string& scriptPath) 
{
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    std::string command = "python \"" + scriptPath + "\"";

    if (!CreateProcessA(
        NULL,
        &command[0], // mutable string buffer
        NULL,
        NULL,
        FALSE,
        CREATE_NO_WINDOW, // no window for background running
        NULL,
        NULL,
        &si,
        &pi
    )) {
        std::cerr << "[ERROR] Failed to start Python script. Error: " << GetLastError() << "\n";
        ZeroMemory(&pi, sizeof(pi));
    }

    return pi;
}

void StopPythonScript(PROCESS_INFORMATION& pi) 
{
    if (pi.hProcess) 
    {
        TerminateProcess(pi.hProcess, 0);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
}

std::string GetCurrentDirectoryPath()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string fullPath(buffer);
    return fullPath.substr(0, fullPath.find_last_of("\\/"));
}

// ------------------------------------------------------------------------- \\
// -------------------------------ExtBytePatch------------------------------ \\
// ------------------------------------------------------------------------- \\ 

void MsgBoxAddy(uintptr_t addy)
{
    char szAddressOnly[32];
    sprintf_s(szAddressOnly, "%016llX", addy);

    const size_t len = strlen(szAddressOnly) + 1;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    if (hMem)
    {
        void* pMem = GlobalLock(hMem);
        if (pMem)  // Check if GlobalLock succeeded
        {
            memcpy(pMem, szAddressOnly, len);
            GlobalUnlock(hMem);
            OpenClipboard(0);
            EmptyClipboard();
            SetClipboardData(CF_TEXT, hMem);
            CloseClipboard();
        }
        else
        {
            // Handle error: GlobalLock failed
            MessageBoxA(NULL, "Failed to lock global memory.", "Error", MB_OK);
        }
    }
    else
    {
        // Handle error: GlobalAlloc failed
        MessageBoxA(NULL, "Failed to allocate global memory.", "Error", MB_OK);
    }

    char szFullMsg[64];
    sprintf_s(szFullMsg, "Address copied: %s", szAddressOnly);
    MessageBoxA(NULL, szFullMsg, "Copied!", MB_OK);
}

DWORD GetProcessIdByName(const std::string& processName)
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        return 0;

    if (Process32First(snapshot, &entry))
    {
        do
        {
            if (processName == entry.szExeFile)
            {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return 0;
}

uintptr_t GetBaseAddress(DWORD processId)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (!hProcess)
        return 0;

    HMODULE hMod[1024];
    DWORD cbNeeded;
    if (EnumProcessModules(hProcess, hMod, sizeof(hMod), &cbNeeded))
    {
        uintptr_t baseAddress = (uintptr_t)hMod[0];
        CloseHandle(hProcess);
        return baseAddress;
    }

    CloseHandle(hProcess);
    return 0;
}

bool ReadByte(HANDLE hProcess, uintptr_t address, uint8_t& outByte)
{
    SIZE_T bytesRead;
    return ReadProcessMemory(hProcess, (LPCVOID)address, &outByte, sizeof(outByte), &bytesRead) && bytesRead == sizeof(outByte);
}

void WriteByte(HANDLE hProcess, uintptr_t address, uint8_t value)
{
    DWORD oldProtect;
    VirtualProtectEx(hProcess, (LPVOID)address, sizeof(value), PAGE_EXECUTE_READWRITE, &oldProtect);
    SIZE_T bytesWritten;
    WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(value), &bytesWritten);
    VirtualProtectEx(hProcess, (LPVOID)address, sizeof(value), oldProtect, &oldProtect);
}

size_t GetModuleSize(const char* moduleName)
{
    DWORD processId = GetProcessIdByName("RainbowSix.exe");
    if (!processId) return 0;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
    if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

    MODULEENTRY32 me32 = { 0 };
    me32.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(hSnapshot, &me32))
    {
        do
        {
            if (_stricmp(me32.szModule, moduleName) == 0)
            {
                CloseHandle(hSnapshot);
                return (size_t)me32.modBaseSize;
            }
        } while (Module32Next(hSnapshot, &me32));
    }

    CloseHandle(hSnapshot);
    return 0;
}

uintptr_t FindPatternExternal(HANDLE hProcess, uintptr_t base, size_t size, const char* pattern, const char* mask)
{
    std::vector<char> buffer(size);
    SIZE_T bytesRead;
    if (!ReadProcessMemory(hProcess, (LPCVOID)base, buffer.data(), size, &bytesRead))
    {
        MessageBoxA(NULL, "ReadProcessMemory failed!", "Error", MB_OK);
        return 0;
    }
    if (bytesRead == 0)
    {
        MessageBoxA(NULL, "No bytes read!", "Error", MB_OK);
        return 0;
    }

    size_t patternLen = strlen(mask);

    for (size_t i = 0; i < bytesRead - patternLen; i++)
    {
        bool found = true;
        for (size_t j = 0; j < patternLen; j++)
        {
            if (mask[j] != '?' && pattern[j] != buffer[i + j])
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            return base + i;
        }
    }

    return 0;
}

// ------------------------------------------------------------------------- \\
// ------------------------------Cheat Functions---------------------------- \\
// ------------------------------------------------------------------------- \\ 

int GlowESP()
{
    DWORD processId = GetProcessIdByName("RainbowSix.exe");
    if (processId == 0) return 1;

    uintptr_t baseAddress = GetBaseAddress(processId);
    if (baseAddress == 0) return 1;

    uintptr_t addressToPatch1 = baseAddress + 0x375b773;
    uintptr_t addressToPatch2 = baseAddress + 0x375be7c;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!hProcess) return 1;

    uint8_t currentByte;
    if (!ReadByte(hProcess, addressToPatch1, currentByte))
    {
        CloseHandle(hProcess);
        return 1;
    }

    uint8_t newValue = (currentByte == 0x74) ? 0x75 : 0x74;
    WriteByte(hProcess, addressToPatch1, newValue);
    WriteByte(hProcess, addressToPatch2, newValue);

    CloseHandle(hProcess);
    return 0;
}

int RunShoot(uintptr_t patchAddr1, uintptr_t patchAddr2)
{
    DWORD processId = GetProcessIdByName("RainbowSix.exe");
    if (processId == 0) return 1;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!hProcess) return 1;

    uintptr_t baseAddress = GetBaseAddress(processId);
    size_t moduleSize = GetModuleSize("RainbowSix.exe");

    // Read 7 bytes from patchAddr1 to get current compare value
    BYTE patchBytes[7];
    if (!ReadProcessMemory(hProcess, (LPCVOID)patchAddr1, &patchBytes, sizeof(patchBytes), nullptr))
    {
        std::cerr << "[!] Failed to read patch bytes.\n";
        CloseHandle(hProcess);
        return 1;
    }

    // Toggle the compare value
    BYTE currentValue = patchBytes[6];
    BYTE newValue = (currentValue == 0x01) ? 0x00 : 0x01;
    patchBytes[6] = newValue;

    WriteProcessMemory(hProcess, (LPVOID)patchAddr1, patchBytes, sizeof(patchBytes), nullptr);
    WriteProcessMemory(hProcess, (LPVOID)patchAddr2, patchBytes, sizeof(patchBytes), nullptr);

    CloseHandle(hProcess);
    return 0;
}

int BoltScript()
{
    DWORD processId = GetProcessIdByName("RainbowSix.exe");
    if (processId == 0) return 1;

    uintptr_t baseAddress = GetBaseAddress(processId);
    if (baseAddress == 0) return 1;

    uintptr_t patchAddress = baseAddress + 0x1c7e2d1; // Target instruction

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!hProcess) return 1;

    // Read current byte at [patchAddress + 6] to determine if patch is applied
    uint8_t currentValue = 0;
    if (!ReadByte(hProcess, patchAddress + 6, currentValue)) // +6 = the immediate value (00 or 01)
    {
        CloseHandle(hProcess);
        return 1;
    }

    // Toggle between 0 and 1
    uint8_t newValue = (currentValue == 0x00) ? 0x01 : 0x00;

    // Write the full opcode: mov byte ptr [rax+92], newValue
    uint8_t patchBytes[7] = { 0xC6, 0x80, 0x92, 0x00, 0x00, 0x00, newValue };
    SIZE_T bytesWritten = 0;
    WriteProcessMemory(hProcess, (LPVOID)patchAddress, patchBytes, sizeof(patchBytes), &bytesWritten);

    CloseHandle(hProcess);
    return (bytesWritten == sizeof(patchBytes)) ? 0 : 1;
}

int InfiniteSecondaryGadgets()
{
    DWORD processId = GetProcessIdByName("RainbowSix.exe");
    if (processId == 0) return 1;

    uintptr_t baseAddress = GetBaseAddress(processId);
    if (baseAddress == 0) return 1;

    uintptr_t patchAddress = baseAddress + 0x319BD38;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!hProcess) return 1;

    // Read original bytes (3 bytes)
    uint8_t originalBytes[3];
    if (!ReadProcessMemory(hProcess, (LPCVOID)patchAddress, &originalBytes, 3, nullptr))
    {
        CloseHandle(hProcess);
        return 1;
    }

    // Check if already NOPed
    bool isNOPed = (originalBytes[0] == 0x90 && originalBytes[1] == 0x90 && originalBytes[2] == 0x90);

    if (isNOPed)
    {
        // Restore original instruction: mov [rcx+64], edi -> 89 79 64
        uint8_t restoreInstruction[3] = { 0x89, 0x79, 0x64 };
        WriteProcessMemory(hProcess, (LPVOID)patchAddress, restoreInstruction, 3, nullptr);
    }
    else
    {
        // NOP out the instruction
        uint8_t nops[3] = { 0x90, 0x90, 0x90 };
        WriteProcessMemory(hProcess, (LPVOID)patchAddress, nops, 3, nullptr);
    }

    CloseHandle(hProcess);
    return 0;
}

int GoOutside()
{
    DWORD processId = GetProcessIdByName("RainbowSix.exe");
    if (processId == 0) return 1;

    uintptr_t baseAddress = GetBaseAddress(processId);
    if (baseAddress == 0) return 1;

    uintptr_t patchAddress = baseAddress + 0x3865294;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!hProcess) return 1;

    // Read current bytes (6 bytes)
    uint8_t currentBytes[6];
    if (!ReadProcessMemory(hProcess, (LPCVOID)patchAddress, &currentBytes, 6, nullptr))
    {
        CloseHandle(hProcess);
        return 1;
    }

    // Check if already NOPed
    bool isNOPed = true;
    for (int i = 0; i < 6; i++)
    {
        if (currentBytes[i] != 0x90)
        {
            isNOPed = false;
            break;
        }
    }

    if (isNOPed)
    {
        // Restore: mov eax,[rax+5118] -> 8B 80 18 51 00 00
        uint8_t originalInstruction[6] = { 0x8B, 0x80, 0x18, 0x51, 0x00, 0x00 };
        WriteProcessMemory(hProcess, (LPVOID)patchAddress, originalInstruction, 6, nullptr);
    }
    else
    {
        // NOP 6 bytes
        uint8_t nops[6] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
        WriteProcessMemory(hProcess, (LPVOID)patchAddress, nops, 6, nullptr);
    }

    CloseHandle(hProcess);
    return 0;
}

int UnlockAll()
{
    DWORD processId = GetProcessIdByName("RainbowSix.exe");
    if (processId == 0) return 1;

    uintptr_t baseAddress = GetBaseAddress(processId);
    if (baseAddress == 0) return 1;

    uintptr_t patchAddress = baseAddress + 0x3843080;
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!hProcess) return 1;

    // Patch: mov r12l, 00; nop
    uint8_t patchBytes[4] = { 0x41, 0xB4, 0x00, 0x90 };
    WriteProcessMemory(hProcess, (LPVOID)patchAddress, patchBytes, 4, nullptr);

    CloseHandle(hProcess);
    return 0;
}
