#pragma once
#include <string>
#include <functional>

class Address
{
public:
	std::wstring Get();
	void Set(const std::wstring& addr);
	void AddInput(const wchar_t& input);
	void Chop();
	void SetOnChanged(std::function<void()> callback);
	void Complete();
private:
	std::wstring data;
	std::function<void()> callback;
	void OnChanged();
};

