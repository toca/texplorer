#pragma once
#include <string>
#include <functional>

class Address
{
public:
	std::wstring Get();
	void AddInput(const wchar_t& input);
	void Chop();
	void SetOnChanged(std::function<void()> callback);
private:
	std::wstring data;
	std::function<void()> callback;
	void OnChanged();
};

