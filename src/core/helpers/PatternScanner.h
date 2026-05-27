#pragma once
#include <unordered_map>
#include "../../sdk/helpers/console.h"

class DPatternScanner //Temporary name until I delete the original PatternScanner class...
{
public:
	DPatternScanner() = default;
	DPatternScanner(uintptr_t _base) : base(_base) {}

	DPatternScanner& scan(const std::string_view& signature, const std::string_view& sig_name)
	{
		this->addr = pattern_scan(signature.data());
		if (!this->addr)
			g_Console->println("PatternScanner: %s not found\n", sig_name.data());

		return *this;
	}

	DPatternScanner& add(const uintptr_t& offset)
	{
		if (this->addr)
			this->addr += offset;

		return *this;
	}

	DPatternScanner& abs()
	{
		if (this->addr)
		{
			const int relative = *reinterpret_cast<int*>(this->addr);

			this->addr += relative + sizeof(relative);
		}

		return *this;
	}

	template <typename T = void*>
	T as()
	{
		if (!addr)
			return {};

		return reinterpret_cast<T>(addr);
	}

private:
	//BMH hybrid, not pure BMH
	std::uint8_t* pattern_scan(const char* signature) const
	{
		static auto pattern_to_bytes = [](const char* pattern)
			{
				auto bytes = std::vector<int>{};
				auto start = const_cast<char*>(pattern);
				auto end = const_cast<char*>(pattern) + strlen(pattern);

				for (auto current = start; current < end; ++current)
				{
					if (*current == '?')
					{
						++current;

						if (*current == '?')
							++current;

						bytes.emplace_back(-1);
					}
					else bytes.emplace_back(strtoul(current, &current, 16));
				}
				return bytes;
			};

		auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(this->base);
		auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(dos_header) + dos_header->e_lfanew);
		auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;

		auto pattern_bytes = pattern_to_bytes(signature);
		auto scan_bytes = reinterpret_cast<uint8_t*>(this->base);
		const size_t pattern_len = pattern_bytes.size();

		// Fallback to naive scan if there are wildcards
		if (std::find(pattern_bytes.begin(), pattern_bytes.end(), -1) != pattern_bytes.end())
		{
			for (size_t i = 0; i <= size_of_image - pattern_len; ++i)
			{
				bool found = true;
				for (size_t j = 0; j < pattern_len; ++j)
				{
					if (pattern_bytes[j] != -1 && scan_bytes[i + j] != pattern_bytes[j])
					{
						found = false;
						break;
					}
				}

				if (found)
					return &scan_bytes[i];
			}

			return nullptr;
		}

		// Boyer-Moore-Horspool
		std::unordered_map<uint8_t, size_t> bad_char_shift;
		for (size_t i = 0; i < pattern_len - 1; ++i)
			bad_char_shift[static_cast<uint8_t>(pattern_bytes[i])] = pattern_len - 1 - i;

		size_t i = 0;
		while (i <= size_of_image - pattern_len)
		{
			int j = static_cast<int>(pattern_len) - 1;
			while (j >= 0 && scan_bytes[i + j] == pattern_bytes[j])
				--j;

			if (j < 0)
				return &scan_bytes[i];

			uint8_t next_byte = scan_bytes[i + pattern_len - 1];
			i += bad_char_shift.count(next_byte) ? bad_char_shift[next_byte] : pattern_len;
		}

		return nullptr;
	}

	uintptr_t base;
	uint8_t* addr;
};

