#include "Controller.h"
#include <cwctype>
#include "View.h"
#include "Address.h"
#include "CurrentDir.h"
#include "Notification.h"
#include "Jumper.h"

Controller::Controller()
{
	// create models
	// add key event Listener self
	this->address = std::make_shared<Address>();
	this->currentDir = std::make_shared<CurrentDir>();
	this->notification = std::make_shared<Notification>();
	this->jumper = std::make_shared<Jumper>();

	this->view = std::make_unique<View>(this, address, currentDir, notification, jumper);
	this->address->SetOnChanged([this]() {
		this->view->OnAddressChanged();
	});
	this->currentDir->SetOnChanged([this]() {
		this->view->OnItemChanged();
		this->address->Set(this->currentDir->Absolute());
	});
	this->notification->SetSubscriber([this]() {
		this->view->OnNotificationChanged();
	});
	this->jumper->AddSubscriber([this]() {
		this->view->OnStateChanged();
	});
}

void Controller::Start()
{
	this->currentDir->Change(L".");
	this->view->Show();
}

void Controller::Stop()
{
	this->view->Close();
}


void Controller::OnKeyEvent(KEY_EVENT_RECORD keyEvent)
{
	if (!keyEvent.bKeyDown)
	{
		return;
	}
	try
	{
		//std::wstring msg(L"key event: ");
		//msg += std::to_wstring(keyEvent.wVirtualKeyCode) + L"\n";
		//::OutputDebugString(msg.c_str());
		switch (keyEvent.wVirtualKeyCode)
		{
		case VK_BACK:
			this->address->Chop();
			break;
		case VK_RETURN:
			if (this->notification->ErrorOccurred())
			{
				this->notification->Clear();
			}
			else
			{
				this->currentDir->Change(this->address->Get());
			}
			break;
		case VK_SPACE:
			this->notification->Clear();
			break;
		case VK_UP:
			this->view->Up();
			break;
		case VK_DOWN:
			this->view->Down();
			break;
		case VK_LEFT:
			this->currentDir->Up();
			this->view->Select(0);
			break;
		case VK_RIGHT:
			this->currentDir->Into(this->view->Selected());
			this->view->Select(0);
			break;
		case VK_ESCAPE:
			this->Stop();
			break;
		case VK_TAB:
			this->address->Complete();
			break;
		default:
			if (keyEvent.uChar.UnicodeChar != L'\0')
			{
				if (this->jumper->Crouching())
				{
					auto index = this->jumper->Find(keyEvent.uChar.UnicodeChar, this->currentDir->GetItems());
					this->view->Select(index);
				}
				else
				{
					this->address->AddInput(keyEvent.uChar.UnicodeChar);
				}
			}
			// open
			if (this->pressedWithCtrl(L'o', keyEvent))
			{
				// if ShellExecute directory then conhost will hide by vs code
				auto path = this->currentDir->Get(this->view->Selected());
				std::wstring params = L"/c start " + path.wstring();
				auto result = ::ShellExecute(nullptr, L"open", L"cmd.exe", params.c_str(), nullptr, SW_HIDE);
				auto le = GetLastError();
			}
			// sort
			if (this->pressedWithCtrl(L'n', keyEvent))
			{
				this->currentDir->SortName();
			}
			if (this->pressedWithCtrl(L'd', keyEvent))
			{
				this->currentDir->SortDate();
			}
			if (this->pressedWithCtrl(L's', keyEvent))
			{
				this->currentDir->SortSize();
			}
			// jump
			if (this->pressedWithCtrl(L'g', keyEvent))
			{
				this->jumper->Ready();
			}
			break;
		}
	}
	catch (std::exception& ex)
	{
		auto bufSize = ::MultiByteToWideChar(CP_ACP, 0, ex.what(), -1, nullptr, 0);
		std::wstring message(bufSize, L'\0');
		::MultiByteToWideChar(CP_ACP, 0, ex.what(), -1, message.data(), bufSize);
		this->notification->OnError(message);
	}
}

std::wstring Controller::GetCurrentDir()
{
	return this->currentDir->Absolute().wstring();
}

bool Controller::pressedWithCtrl(wchar_t key, const KEY_EVENT_RECORD& keyEvent)
{
	wchar_t upperKey = std::towupper(key);
	// capital letter == virtual key code
	//static const WORD VK_A = 0x41; //WORD keyCode = VK_A + (key - L'a');
	if ((keyEvent.dwControlKeyState & LEFT_CTRL_PRESSED) || (keyEvent.dwControlKeyState & RIGHT_CTRL_PRESSED))
	{
		if (upperKey == keyEvent.wVirtualKeyCode)
		{
			return true;
		}
	}
	return false;
}
