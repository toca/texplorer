#include "View.h"
#include "Controller.h"
#include "Address.h"
#include "HorizontalBox.h"


View::View(Controller* controller, std::shared_ptr<Address> address)
	:controller(controller)
	,tuindow(std::make_unique<tuindow::Tuindow>())
	,addressLabel(std::make_shared<tuindow::Label>(L"",  std::make_shared<tuindow::Style>(tuindow::Color::WHITE, tuindow::Color::BLUE)))
	,address(address)
{
	auto hbox = std::make_shared<tuindow::HorizontalBox>();
	auto pathLabel = std::make_shared<tuindow::Label>(L"path: ", tuindow::FixedSize{6, 1});
	hbox->Push(pathLabel);
	hbox->Push(this->addressLabel);
	this->tuindow->Put(hbox);
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
