#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <functional>

class Jumper
{
public:
	std::wstring Message();
	void Ready();
	size_t Find(wchar_t initiali, const std::vector<std::filesystem::directory_entry>& entries);
	bool Crouching();
	void AddSubscriber(std::function<void()> callback);
private:
	bool crouching = false;
	std::function<void()> callback;
};

