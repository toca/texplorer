#pragma once
#include <cstdint>
#include <vector>
#include <Windows.h>

namespace tuindow
{
	class Cell;

	struct ScreenEvent 
	{
		bool HasKeyEvent;
		std::vector<KEY_EVENT_RECORD> KeyEvents;
		bool HasSizeEvent;
		COORD NewSize;
	};

	class Screen
	{
	public:
		Screen();
		~Screen();

		void Clear();
		void Show();
		void Close();
		ScreenEvent ReadInput();

		int Put(const Cell& c, uint32_t x, uint32_t y);
		void SetCursor(short x, short y);

		uint32_t Row();
		uint32_t Col();
	private:
		void OnSizeChanged(COORD newSize);
		void AllocBuffer();
		COORD windowSize;
		COORD cursorPos;
		HANDLE screenBuffers[2];
		std::vector < std::vector<Cell> > lines;
		int32_t screenIndex = 0;
		uint32_t row = 0;
		uint32_t col = 0;
		bool updated = false;

		HANDLE stdouth;
	};

	bool RectEquals(SMALL_RECT lhs, SMALL_RECT rhs);
}

