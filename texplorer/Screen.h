#pragma once
#include <cstdint>
#include <vector>
#include <Windows.h>

namespace tuindow
{
	class Cell;

	class Screen
	{
	public:
		Screen();
		~Screen();

		void Clear();
		void Show();
		std::vector<KEY_EVENT_RECORD> ReadInput();

		void Put(const Cell& c, uint32_t x, uint32_t y);

		uint32_t Row();
		uint32_t Col();
	private:
		HANDLE screenBuffers[2];
		std::vector < std::vector<Cell> > lines;
		int32_t screenIndex = 0;
		uint32_t row = 0;
		uint32_t col = 0;
		bool updated = false;
	};

}

