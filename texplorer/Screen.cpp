#include "Screen.h"
#include <Windows.h>
#include <string>

#include "Cell.h"
#include "CharacterWidth.h"

std::wstring format(const wchar_t* fmt, ...)
{
	va_list arg;

	va_start(arg, fmt);

	std::wstring result;
	result.resize(1024);
	auto len = _vsnwprintf_s(result.data(), result.size(), result.size(), fmt, arg);
	if (len < 0)
	{
		throw std::exception("Error size not enough?");
	}
	result.resize(len);
	va_end(arg);

	return result;
}

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
		this->windowSize = { short(this->col), short(this->row) };
		// screen buffer
		HANDLE buffer1 = ::CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
		HANDLE buffer2 = ::CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
		this->screenBuffers[0] = buffer1;
		this->screenBuffers[1] = buffer2;

		DWORD mode = 0;
		::GetConsoleMode(buffer1, &mode);
		mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		::SetConsoleMode(buffer1, mode);
		::SetConsoleMode(buffer2, mode);
		//CONSOLE_CURSOR_INFO info{};
		//::GetConsoleCursorInfo(buffer1, &info);
		//info.bVisible = FALSE;
		//::SetConsoleCursorInfo(buffer1, &info);
		//::GetConsoleCursorInfo(buffer2, &info);
		//info.bVisible = FALSE;
		//::SetConsoleCursorInfo(buffer2, &info);

		this->AllocBuffer();
		//this->lines = std::vector < std::vector<Cell> >(this->row);
		//for (size_t r = 0; r < this->row; r++)
		//{
		//	this->lines[r] = std::vector<Cell>(this->col);
		//}
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

		std::wstring content(L"");
		std::shared_ptr<Style> prevStyle = std::make_shared<Style>(tuindow::Color::NONE, tuindow::Color::NONE);
		for (auto& line : this->lines)
		{
			for (int c = 0; c < line.size();)
			{
				auto style = line[c].Style();
				auto character = line[c].GetChar();
				if (*style == *prevStyle)
				{
					content += character;
				}
				else
				{
					content += style->PostSequence() + style->PreSequence() + character;
				}
				c += Util::CharacterWidth(character);
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


	ScreenEvent Screen::ReadInput()
	{
		CONSOLE_SCREEN_BUFFER_INFOEX infoEx{};
		infoEx.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		::GetConsoleScreenBufferInfoEx(this->screenBuffers[this->screenIndex], &infoEx);
		if (this->windowSize.X != short(infoEx.srWindow.Right - infoEx.srWindow.Left + 1) ||
			this->windowSize.Y != short(infoEx.srWindow.Bottom - infoEx.srWindow.Top + 1))
		{
			COORD newSize = { short(infoEx.srWindow.Right - infoEx.srWindow.Left + 1), short(infoEx.srWindow.Bottom - infoEx.srWindow.Top + 1) };
			this->windowSize = newSize;
			this->OnSizeChanged(newSize);
			return {
				false,
				std::vector<KEY_EVENT_RECORD>(0),
				true,
				newSize
			};
		}

		DWORD count = 0;
		HANDLE handle = ::GetStdHandle(STD_INPUT_HANDLE);
		::GetNumberOfConsoleInputEvents(handle, &count);
		if (!count)
		{
			return {};
		}
		std::vector<INPUT_RECORD> inputs(4);
		DWORD len = DWORD(inputs.size());
		DWORD numOfEvents = 0;

		::ReadConsoleInput(handle, inputs.data(), len, &numOfEvents);

		ScreenEvent result{
			false,
			std::vector<KEY_EVENT_RECORD>(numOfEvents),
			false,
			{}
		};

		for (size_t i = 0; i < numOfEvents; i++)
		{
			if (inputs[i].EventType == KEY_EVENT)
			{
				result.HasKeyEvent = true;
				result.KeyEvents.push_back(inputs[i].Event.KeyEvent);
			}
			
			if (inputs[i].EventType == WINDOW_BUFFER_SIZE_EVENT)
			{
				auto newSize = inputs[i].Event.WindowBufferSizeEvent.dwSize;
				::OutputDebugString(format(L"newSize: %d, %d\n", newSize.X, newSize.Y).c_str());
				if (this->windowSize.X != newSize.X || this->windowSize.Y != newSize.Y)
				{
					//this->OnSizeChanged(newSize);
					//result.HasSizeEvent = true;
					//result.NewSize = newSize;
				}
			}
		}
		return result;
	}

	
	int Screen::Put(const Cell& c, uint32_t x, uint32_t y)
	{
		// TODO fix buffer size? ‘å‚«–Ú‚Éæ‚Á‚Ä‚¨‚¢‚ÄA•`‰æ‚Å‚«‚é‚È‚ç•`‰æA‚Í‚İ‚¾‚·‚È‚ç–³‹‚·‚é?
		if (y < this->lines.size() && x < this->lines[y].size())
		{
			this->lines[y][x] = c;
			this->updated = true;
		}
		return Util::CharacterWidth(c.GetChar());
	}


	uint32_t Screen::Row()
	{
		return this->row;
	}


	uint32_t Screen::Col()
	{
		return this->col;
	}
	void Screen::OnSizeChanged(COORD newSize)
	{
		// TODO resize this->lines!!!
		if (newSize.Y < 5)  return;

		OutputDebugString(L"x:");
		OutputDebugString(std::to_wstring(newSize.X).c_str());
		OutputDebugString(L"\ny:");
		OutputDebugString(std::to_wstring(newSize.Y).c_str());
		OutputDebugString(L"\n");

		CONSOLE_SCREEN_BUFFER_INFOEX infoEx{};
		infoEx.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		::GetConsoleScreenBufferInfoEx(this->screenBuffers[this->screenIndex], &infoEx);

		infoEx.dwSize = { short(infoEx.srWindow.Right - infoEx.srWindow.Left + 1), short(infoEx.srWindow.Bottom - infoEx.srWindow.Top + 1) };
		// set front buffer size
		BOOL res = ::SetConsoleScreenBufferSize(this->screenBuffers[this->screenIndex], infoEx.dwSize);
		if (!res) {
			auto err = GetLastError();
			::OutputDebugString(format(L"front NG: %d\n", err).c_str());
		}

		//infoEx.dwSize = newSize;
		infoEx.srWindow.Top = 0;
		infoEx.srWindow.Left = 0;
		infoEx.srWindow.Bottom++;// = infoEx.dwSize.Y;
		infoEx.srWindow.Right++;// = infoEx.dwSize.X;
		::SetConsoleScreenBufferInfoEx(this->screenBuffers[this->screenIndex ^ 1], &infoEx);
		//// first alloc enough buffer
		//SMALL_RECT temp{ 0, 0, 10, 1 };
		//res = ::SetConsoleWindowInfo(this->screenBuffers[this->screenIndex ^ 1], TRUE, &temp);
		//if (!res)
		//{
		//	auto err = GetLastError();
		//	OutputDebugString(format(L"NOOOOOOOOOO:%d\n", err).c_str());
		//}

		//CONSOLE_SCREEN_BUFFER_INFOEX backInfoEx{};
		//backInfoEx.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		//res = ::GetConsoleScreenBufferInfoEx(this->screenBuffers[this->screenIndex ^ 1], &backInfoEx);
		//if (!res)
		//{
		//	auto err = GetLastError();
		//	OutputDebugString(format(L"NOOOOOOOOOO:%d\n", err).c_str());
		//}

		//res = ::SetConsoleScreenBufferSize(this->screenBuffers[this->screenIndex ^ 1], newSize);
		//if (!res) {
		//	auto err = GetLastError();
		//	::OutputDebugString(format(L"NG Back %d\n", err).c_str());
		//}
		//else {
		//	::OutputDebugString(L"OK Back\n");

		//}

		////if (!RectEquals(infoEx.srWindow, backInfoEx.srWindow))
		////{
		//	//::OutputDebugString(L"window size not match\n");
		//	if (::SetConsoleWindowInfo(this->screenBuffers[this->screenIndex ^ 1], TRUE, &infoEx.srWindow))
		//	{
		//		::OutputDebugString(L"OK windowsize\n");


		//	}
		//	else {
		//		auto err = GetLastError();
		//		::OutputDebugString(format(L"NG windowsize: %d\n", err).c_str());
		//	}
		////}
		//	
		





		this->windowSize = newSize;
		this->row = newSize.Y;
		this->col = newSize.X;
		this->AllocBuffer();
	}
	void Screen::AllocBuffer()
	{
		this->lines.resize(this->row);
		for (size_t r = 0; r < this->row; r++)
		{
			this->lines[r].resize(this->col);
		}
	}
	bool RectEquals(SMALL_RECT lhs, SMALL_RECT rhs)
	{
		if (lhs.Top != rhs.Top) return false;
		if (lhs.Bottom != rhs.Bottom) return false;
		if (lhs.Left != rhs.Left) return false;
		if (lhs.Right != rhs.Right) return false;
		return true;
	}
}
