#include <locale>
#include <format>
#include <cstdio>
#include <thread>
#include <string>
#include <Windows.h>

#include "Controller.h"

//test
#include <ctime>
#include <iostream>
#include "CharacterWidth.h"

//HANDLE stopEvent;
bool breaked = false;
Controller controller;

int main()
{
	setlocale(LC_ALL, "");

	//printf("│abc┌def\n");
	//std::wstring s = L"\U0001F491\n";
	//std::wcout << s << std::endl;
	//return 0;
	//auto a = tuindow::CharactorWidth(L'あ');
	//auto b = tuindow::CharactorWidth(L'ん');
	//auto c = tuindow::CharactorWidth(L'a');
	//auto d = tuindow::CharactorWidth(L'Z');
	//auto e = tuindow::CharactorWidth(L' ');
	//auto f = tuindow::CharactorWidth(L'\a');
	//auto g = tuindow::CharactorWidth(L'☺');
	//return 0;


	::SetConsoleCtrlHandler([](DWORD event) -> BOOL {
		try
		{
			if (event == CTRL_C_EVENT)
			{
				printf("Break\n");
				//::SetEvent(stopEvent);
				controller.Stop();
				breaked = true;
				return TRUE;
			}
		}
		catch (std::system_error& e)
		{
			printf("Error: %s\n", e.what());
		}
		return FALSE;
		}, TRUE
	);

	controller.Start();

	if (!breaked)
	{
		STARTUPINFO startupInfo{};
		//::GetStartupInfo(&startupInfo);
		PROCESS_INFORMATION procInfo{};
		std::wstring params = L"cmd.exe";
		std::wstring cd = controller.GetCurrentDir();

		auto createProcResult = ::CreateProcessW(
			nullptr,
			const_cast<LPWSTR>(params.c_str()),
			nullptr,
			nullptr,
			FALSE,
			CREATE_NEW_PROCESS_GROUP,
			nullptr,
			cd.c_str(),
			&startupInfo,
			&procInfo
		);
		if (!createProcResult)
		{
			auto lastErr = ::GetLastError();
			printf("lastError:%d\n", lastErr);
			controller.Stop();
			return 1;
		}

		WaitForSingleObject(procInfo.hProcess, INFINITE);
		CloseHandle(procInfo.hProcess);
		CloseHandle(procInfo.hThread);
	}

	controller.Stop();
	return 0;
}
