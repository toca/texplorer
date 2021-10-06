#pragma once
#include <Windows.h>
#include <Memory>
#include "View.h"

// proto
class Address;
class CurrentDir;

class Controller
{
public:
	Controller();
	~Controller() = default;

	void Start();
	void Stop();
	void OnKeyEvent(KEY_EVENT_RECORD keyEvent);
private:
	std::unique_ptr<View> view;
	std::shared_ptr<Address> address;
	std::shared_ptr<CurrentDir> currentDir;
};
