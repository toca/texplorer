#pragma once

#include <memory>
#include "Tuindow.h"
#include "Label.h"

class Controller;
class Address;

class View
{
public:
	View(Controller* controller, std::shared_ptr<Address> address);
	~View() = default;

	void Show();
	void Close();

	void OnAddressChanged();
private:
	Controller* controller;
	std::unique_ptr<tuindow::Tuindow> tuindow;

	std::shared_ptr<tuindow::Label> addressLabel;
	std::shared_ptr<Address> address;

	void OnKeyEvent(KEY_EVENT_RECORD keyEvent);
};
