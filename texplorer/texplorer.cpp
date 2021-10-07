#include <locale>
#include <format>
#include <cstdio>
#include <thread>
#include <string>
#include <Windows.h>

#include "Controller.h"

//test
#include <ctime>
#include "CharacterWidth.h"

HANDLE stopEvent;
bool stop = false;
int main()
{
	setlocale(LC_ALL, "");

	//auto a = tuindow::CharactorWidth(L'あ');
	//auto b = tuindow::CharactorWidth(L'ん');
	//auto c = tuindow::CharactorWidth(L'a');
	//auto d = tuindow::CharactorWidth(L'Z');
	//auto e = tuindow::CharactorWidth(L' ');
	//auto f = tuindow::CharactorWidth(L'\a');
	//auto g = tuindow::CharactorWidth(L'☺');
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
