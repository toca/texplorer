#pragma once
#include <cstdint>
#include <vector>
#include <Windows.h>

namespace tuindow
{
	class Character;

	class Screen
	{
	public:
		Screen();
		~Screen();

		void Clear();
		void Show();
		KEY_EVENT_RECORD ReadInput();

		void Put(const Character& c, uint32_t x, uint32_t y);

		uint32_t Row();
		uint32_t Col();
	private:
		HANDLE screenBuffers[2];
		std::vector < std::vector<Character> > lines;
		int32_t screenIndex = 0;
		uint32_t row = 0;
		uint32_t col = 0;
		bool updated = false;
	};

}

