#pragma once

#include <memory>
#include "Tuindow.h"
#include "Label.h"
#include "SelectableList.h"

class Controller;
class Address;
class CurrentDir;
class Notification;
class Jumper;

class View
{
public:
	View(Controller* controller, std::shared_ptr<Address> address, std::shared_ptr<CurrentDir> dir, std::shared_ptr<Notification> notification, std::shared_ptr<Jumper> jumper);
	~View() = default;

	void Show();
	void Close();

	void OnNotificationChanged();

	void OnAddressChanged();
	void OnItemChanged();
	void OnStateChanged();

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
	std::shared_ptr<tuindow::Label> number;
	size_t itemCount = 0;
	std::shared_ptr<tuindow::Label> jumpMessage;
	std::shared_ptr<Jumper> jumper;
	std::shared_ptr<CurrentDir> curDir;
	std::shared_ptr<Notification> notification;

	void OnKeyEvent(KEY_EVENT_RECORD keyEvent);
};
