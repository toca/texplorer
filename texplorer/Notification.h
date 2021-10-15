#pragma once
#include <string>
#include <functional>

class Notification
{
public:
	void OnError(const std::wstring& message);
	std::wstring Message();
	bool ErrorOccurred();
	void Clear();
	void SetSubscriber(std::function<void()> callback);
private:
	std::wstring message = L"";
	bool occurred = false;
	std::function<void()> callback;
};

