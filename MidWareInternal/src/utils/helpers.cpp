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