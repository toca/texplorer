#include "Controller.h"
#include "View.h"
#include "Address.h"

Controller::Controller()
{
	// create models
	// add key event Listener self
	this->address = std::make_shared<Address>();
	this->view = std::make_unique<View>(this, address);
	this->address->SetOnChanged([this]() {
		this->view->OnAddressChanged();
	});
}

void Controller::Start()
{
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
		std::wstring msg(L"key event: ");
		msg += std::to_wstring(keyEvent.wVirtualKeyCode) + L"\n";
		::OutputDebugString(msg.c_str());
		switch (keyEvent.wVirtualKeyCode)
		{
		case VK_BACK:
			this->address->Chop();
			break;
		case VK_RETURN:
			OutputDebugString(L"Enter!!!!");
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