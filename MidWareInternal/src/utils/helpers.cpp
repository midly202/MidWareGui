#include "helpers.h"

std::string WideToNarrow(const std::wstring& wstr)
{
    if (wstr.empty()) return std::string();

    size_t sizeRequired = 0;

    // First call to get the required size
    errno_t err = wcstombs_s(&sizeRequired, nullptr, 0, wstr.c_str(), 0);
    if (err != 0) {
        throw std::runtime_error("Failed to get size for wide-to-narrow conversion");
    }

    std::string result(sizeRequired, '\0');

    // Second call to perform the actual conversion
    err = wcstombs_s(nullptr, &result[0], sizeRequired, wstr.c_str(), sizeRequired);
    if (err != 0) {
        throw std::runtime_error("Failed to convert wide string to narrow string");
    }

    // Remove the null terminator added by wcstombs_s
    result.pop_back();

    return result;
}

std::wstring NarrowToWide(const std::string& str)
{
    if (str.empty()) return std::wstring();

    size_t sizeRequired = 0;

    // First call to get required size (including null terminator)
    errno_t err = mbstowcs_s(&sizeRequired, nullptr, 0, str.c_str(), 0);
    if (err != 0) {
        throw std::runtime_error("Failed to get size for narrow-to-wide conversion");
    }

    std::wstring result(sizeRequired, L'\0');

    // Second call to perform conversion
    err = mbstowcs_s(nullptr, &result[0], sizeRequired, str.c_str(), sizeRequired);
    if (err != 0) {
        throw std::runtime_error("Failed to convert narrow string to wide string");
    }

    // Remove the null terminator added by mbstowcs_s
    result.pop_back();

    return result;
}

uintptr_t GetPointer(uintptr_t base, const std::vector<uintptr_t>& offsets)
{
	uintptr_t addr = base;
	MEMORY_BASIC_INFORMATION mbi;

	for (auto offset : offsets)
	{
		uintptr_t ptrToCheck = addr + offset;

		if (!VirtualQuery((void*)ptrToCheck, &mbi, sizeof(mbi)) || mbi.State != MEM_COMMIT)
			return 0; // invalid memory, abort

		addr = *(uintptr_t*)ptrToCheck;
	}
	return addr;
}

int MapCaliberIDToIndex(uint32_t caliberID)
{
    switch (caliberID) {
    case 3864566559:
        return 0; // "Regular Rifle"
    case 1635385480:
        return 1; // "Regular SMG"
    case 224916230:
        return 2; // "Regular Pistol"
    case 2249646560:
        return 3; // "Tachanka's DP27"
    case 54247478:
        return 4; // "Tachanka's DP27 Turret"
    case 1216095593:
        return 5; // "Buck's Skeleton Key"
    case 3683118896:
        return 6; // "Kali's Sniper"
    case 2097060903:
        return 7; // "Maverick's Blowtorch"
    case 1800674797:
        return 8; // "Maestro's Turret"
    case 1586594484:
        return 9; // "BOSG"
    case 1314319139:
        return 10; // "Bailiff"
    default:
        return 11; // "Invalid Caliber"
    }
}