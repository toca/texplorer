#pragma once
#include <vector>
#include <array>
#include <string>

namespace tuindow
{
	using Table = std::vector<std::array<uint32_t, 2>>;
	class Util
	{
	public:
		static int CharacterWidth(const wchar_t c)
		{
			if (c < 0x0020)
			{
				return 0;
			}
			if (inTable(c, ambiguous))
			{
				return 2;
			}
			if (0x3000 < c) {
				return 2;
			}
			return 1;
		}
		static size_t TextWidth(const std::wstring& text);
	private:
		static bool inTable(const wchar_t c, Table table) 
		{
			if (c < table[0][0])
			{
				return false;
			}

			size_t bot = 0;
			size_t top = table.size() - 1;
			while (top >= bot)
			{
				size_t mid = (bot + top) >> 1;
				if (table[mid][1] < c)
				{
					bot = mid + 1;
				}
				else if (table[mid][0] > c)
				{
					top = mid - 1;
				}
				else 
				{
					return true;
				}
			}
			return false;
		}

		static std::vector<std::array<uint32_t, 2>> ambiguous;
	};
}
