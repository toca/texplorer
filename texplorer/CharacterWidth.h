#pragma once

namespace tuindow
{
	class Util
	{
	public:
		static int CharacterWidth(const wchar_t c)
		{
			if (c < 0x0020)
			{
				return 0;
			}
			if (0x3000 < c) {
				return 2;
			}
			return 1;
		}
	};
}