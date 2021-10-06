#pragma once
#include <filesystem>
#include <functional>

class CurrentDir
{
public:
	void Change(const std::filesystem::path cd);
	std::filesystem::path Absolute();
	std::vector<std::filesystem::directory_entry> GetItems();
	void SetOnChanged(std::function<void()> callback);
private:
	std::filesystem::path cd;
	std::function<void()> callback;
};

