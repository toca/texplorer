#include "Controller.h"
#include "View.h"
#include "Address.h"
#include "CurrentDir.h"

Controller::Controller()
{
	// create models
	// add key event Listener self
	this->address = std::make_shared<Address>();
	this->currentDir = std::make_shared<CurrentDir>();

	this->view = std::make_unique<View>(this, address, currentDir);
	this->address->SetOnChanged([this]() {
		this->view->OnAddressChanged();
	});
	this->currentDir->SetOnChanged([this]() {
		this->view->OnItemChanged();
		this->address->Set(this->currentDir->Absolute());
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
	if (keyEvent.bKeyDown)
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
			this->currentDir->Change(this->address->Get());
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
			this->view->Select(0); this->view->Select(0);
			break;
		case VK_ESCAPE:
			this->currentDir->SyncProcCurrentDir();
			this->Stop();
			break;
		default:
			if (keyEvent.uChar.UnicodeChar != L'\0')
			{
				this->address->AddInput(keyEvent.uChar.UnicodeChar);
			}
			break;
		}
	}

	//for (auto& listener : this->keyEventListeners)
	//{
	//	listener(keyEvent);
	//}
}

std::wstring Controller::GetCurrentDir()
{
	return this->currentDir->Absolute().wstring();
}
