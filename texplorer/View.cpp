#include "View.h"
#include "Controller.h"
#include "Address.h"


View::View(Controller* controller, std::shared_ptr<Address> address)
	:controller(controller)
	,tuindow(std::make_unique<tuindow::Tuindow>())
	,addressLabel(std::make_shared<tuindow::Label>(L""))
	,address(address)
{
	this->tuindow->Push(this->addressLabel);
	this->tuindow->AddKeyEventListener([this](auto keyEvent) {
		this->OnKeyEvent(keyEvent);
	});
}

void View::Show()
{
	this->tuindow->Open();
}

void View::Close()
{
	this->tuindow->Close();
}

void View::OnAddressChanged()
{
	this->addressLabel->Set(this->address->Get());
}

void View::OnKeyEvent(KEY_EVENT_RECORD keyEvent)
{
	this->controller->OnKeyEvent(keyEvent);
}
