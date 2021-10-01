#include <locale>
#include <format>
#include <cstdio>
#include <thread>
#include <string>
#include <Windows.h>

#include "Controller.h"

//test
#include <ctime>

HANDLE stopEvent;
bool stop = false;
int main()
{
	setlocale(LC_ALL, "");
	//// test
	//std::thread th([]() {
	//	while (!stop) {
	//		Sleep(10);
	//		DWORD count = 0;
	//			::GetNumberOfConsoleInputEvents(::GetStdHandle(STD_INPUT_HANDLE), &count);
	//		if (!count)
	//		{
	//			continue;
	//		}
	//		INPUT_RECORD input{};
	//		DWORD len = 1;
	//		DWORD numOfEvents = 0;
	//		::ReadConsoleInput(::GetStdHandle(STD_INPUT_HANDLE), &input, len, &numOfEvents);
	//		// TODO resize event
	//		if (input.EventType != KEY_EVENT && input.EventType != MOUSE_EVENT)
	//		{
	//			continue;
	//		}
	//		if (input.EventType == KEY_EVENT)
	//		{
	//			OutputDebugString(L"input: ");
	//			OutputDebugString(std::to_wstring(input.Event.KeyEvent.wVirtualKeyCode).c_str());
	//			OutputDebugString(L"\n");
	//		}
	//	}
	//});
	////wchar_t buf[256];
	////wscanf_s(L"%s", buf, 256);
	//////Sleep(30000);

	//wchar_t buffer[256];
	//ZeroMemory(&buffer, 256);
	//DWORD res = 0;
	//ReadConsole(GetStdHandle(STD_INPUT_HANDLE), buffer, 256, &res, nullptr);
	//wprintf(L"%s\n", buffer);
	//Sleep(5000);
	//stop = true;
	//th.join();
	//return 0;

	// TODO refactor
	stopEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (!stopEvent)
	{
		DWORD lastErr = ::GetLastError();
		std::system_error err(std::error_code(lastErr, std::system_category()), "failed to CreateEvent");
		printf("%s\n", err.what());
		return 1;
	}
	::SetConsoleCtrlHandler([](DWORD event) -> BOOL {
		try
		{
			if (event == CTRL_C_EVENT)
			{
				printf("Break\n");
				::SetEvent(stopEvent);
				stop = true;
				return TRUE;
			}
		}
		catch (std::system_error& e)
		{
			printf("Error: %s\n", e.what());
		}
		return FALSE;
	}, TRUE);



	/*tuindow::Tuindow t;
	auto label = std::make_shared<tuindow::Label>(L"hello world");
	t.Push(label);*/
	/*std::thread th = std::thread([&label]() {
		while (!stop)
		{
			time_t rawtime = time(nullptr);
			tm timeinfo{};
			localtime_s(&timeinfo, &rawtime);
			wchar_t buffer[256];

			::wcsftime(buffer, 256, L"%H:%M:%S", &timeinfo);
			Sleep(100);
			size_t wcsftime(
				wchar_t* strDest,
				size_t maxsize,
				const wchar_t* format,
				const struct tm* timeptr
			);

			label->Set(buffer);
		}
	});*/

	Controller controller;
	controller.Start();
	::WaitForSingleObject(stopEvent, INFINITE);
	controller.Stop();
}
