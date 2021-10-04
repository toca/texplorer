#include "Screen.h"
#include <Windows.h>
#include <string>

#include "Cell.h"

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
		this->lines = std::vector < std::vector<Cell> >(this->row);
		for (size_t r = 0; r < this->row; r++)
		{
			this->lines[r] = std::vector<Cell>(this->col);
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
		std::shared_ptr<Style> prevStyle = std::make_shared<Style>(tuindow::Color::NONE, tuindow::Color::NONE);
		for (auto& line : this->lines)
		{
			for (auto& c : line)
			{
				auto style = c.Style();
				if (*style == *prevStyle)
				{
					content += style->PreSequence() + c.GetChar();
				}
				else
				{
					content = style->PostSequence() + style->PreSequence() + c.GetChar();
				}
				prevStyle = style;
			}
		}

		HANDLE back = this->screenBuffers[this->screenIndex ^ 1];
		DWORD written = 0;

		// reset cursor position
		::SetConsoleCursorPosition(back, { 0, 0 });
		::WriteConsoleW(back, content.data(), content.size(), &written, nullptr);
		::SetConsoleCursorPosition(back, { 1, 10 });
		::SetConsoleActiveScreenBuffer(back);
		this->screenIndex ^= 1;

	}


	std::vector<KEY_EVENT_RECORD> Screen::ReadInput()
	{
		DWORD count = 0;
		::GetNumberOfConsoleInputEvents(::GetStdHandle(STD_INPUT_HANDLE), &count);
		if (!count)
		{
			return {};
		}
		std::vector<INPUT_RECORD> inputs(4);
		DWORD len = DWORD(inputs.size());
		DWORD numOfEvents = 0;

		::ReadConsoleInput(::GetStdHandle(STD_INPUT_HANDLE), inputs.data(), len, &numOfEvents);
		std::vector<KEY_EVENT_RECORD> result(numOfEvents);

		// TODO impl resize event
		for (size_t i = 0; i < numOfEvents; i++)
		{
			if (inputs[i].EventType == KEY_EVENT)
			{
				result.push_back(inputs[i].Event.KeyEvent);
			}
		}
		return result;
	}

	
	void Screen::Put(const Cell& c, uint32_t x, uint32_t y)
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
