#include "Screen.h"
#include <Windows.h>
#include <string>

#include "Character.h"

namespace tuindow
{
	Screen::Screen()
	{
		//// hide cursor
		HANDLE stdOutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
		//CONSOLE_CURSOR_INFO info{};
		//::GetConsoleCursorInfo(stdOutHandle, &info);
		//info.bVisible = FALSE;
		//::SetConsoleCursorInfo(stdOutHandle, &info);

		// size
		CONSOLE_SCREEN_BUFFER_INFOEX infoEx{};
		infoEx.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		::GetConsoleScreenBufferInfoEx(stdOutHandle, &infoEx);

		this->col = infoEx.srWindow.Right + 1;
		this->row = infoEx.srWindow.Bottom + 1;

		// screen buffer
		HANDLE buffer1 = ::CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
		HANDLE buffer2 = ::CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
		this->screenBuffers[0] = buffer1;
		this->screenBuffers[1] = buffer2;
		//CONSOLE_CURSOR_INFO info{};
		//::GetConsoleCursorInfo(buffer1, &info);
		//info.bVisible = FALSE;
		//::SetConsoleCursorInfo(buffer1, &info);
		//::GetConsoleCursorInfo(buffer2, &info);
		//info.bVisible = FALSE;
		//::SetConsoleCursorInfo(buffer2, &info);
		this->lines = std::vector < std::vector<Character> >(this->row);
		for (size_t r = 0; r < this->row; r++)
		{
			this->lines[r] = std::vector<Character>(this->col);
		}
		//auto res = SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
	}


	Screen::~Screen()
	{
	}



	void Screen::Clear()
	{

	}


	void Screen::Show()
	{
		if (!this->updated)
		{
			return;
		}
		this->updated = false;

		// TODO get character width
		std::wstring content(L"");
		for (auto& line : this->lines)
		{
			for (auto& c : line)
			{
				content += c.Get();
			}
			//content += L'\n';
		}

		HANDLE back = this->screenBuffers[this->screenIndex ^ 1];
		DWORD written = 0;

		// reset cursor position
		::SetConsoleCursorPosition(back, { 0, 0 });
		::WriteConsoleW(back, content.data(), content.size(), &written, nullptr);
		::SetConsoleActiveScreenBuffer(back);
		this->screenIndex ^= 1;

		::SetConsoleCursorPosition(back, { 1, 10 });
	}


	KEY_EVENT_RECORD Screen::ReadInput()
	{
		DWORD count = 0;
		::GetNumberOfConsoleInputEvents(::GetStdHandle(STD_INPUT_HANDLE), &count);
		if (!count)
		{
			return {};
		}
		INPUT_RECORD input{};
		DWORD len = 1;
		DWORD numOfEvents = 0;
		::ReadConsoleInput(::GetStdHandle(STD_INPUT_HANDLE), &input, len, &numOfEvents);
		// TODO resize event
		if (input.EventType != KEY_EVENT && input.EventType != MOUSE_EVENT)
		{
			return {};
		}
		if (input.EventType == KEY_EVENT)
		{
			return input.Event.KeyEvent;
		}
		else
		{
			return {};
		}
	}

	
	void Screen::Put(const Character& c, uint32_t x, uint32_t y)
	{
		this->lines[y][x] = c;
		this->updated = true;
	}


	uint32_t Screen::Row()
	{
		return this->row;
	}


	uint32_t Screen::Col()
	{
		return this->col;
	}
}
