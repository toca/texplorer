#include "Notification.h"

void Notification::OnError(const std::wstring& message)
{
	this->message = message;
	this->occurred = true;
	if (this->callback)
	{
		this->callback();
	}
}

std::wstring Notification::Message()
{
	return this->message;
}

bool Notification::ErrorOccurred()
{
	return this->occurred;
}

void Notification::Clear()
{
	this->occurred = false;
	this->message = L"";
	if (this->callback)
	{
		this->callback();
	}
}

void Notification::SetSubscriber(std::function<void()> callback)
{
	this->callback = callback;
}
