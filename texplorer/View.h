#pragma once

#include <memory>
#include "Tuindow.h"
#include "Label.h"
#include "SelectableList.h"

class Controller;
class Address;
class CurrentDir;


class View
{
public:
	View(Controller* controller, std::shared_ptr<Address> address, std::shared_ptr<CurrentDir> dir);
	~View() = default;

	void Show();
	void Close();

	void OnAddressChanged();
	void OnItemChanged();

	void Up();
	void Down();

	void Select(int index);
	int Selected();
private:
	Controller* controller;
	std::unique_ptr<tuindow::Tuindow> tuindow;

	std::shared_ptr<tuindow::Label> addressLabel;
	std::shared_ptr<Address> address;
	std::shared_ptr<tuindow::SelectableList> items;
	std::shared_ptr<tuindow::SelectableList> sizes;
	std::shared_ptr<tuindow::SelectableList> dates;
	std::shared_ptr<CurrentDir> curDir;

	void OnKeyEvent(KEY_EVENT_RECORD keyEvent);
};
