#pragma once
#include <filesystem>
#include <functional>

class CurrentDir
{
public:
	void Change(const std::filesystem::path cd);
	void Up();
	void Into(int index);
	std::filesystem::path Absolute();
	std::vector<std::filesystem::directory_entry> GetItems();
	void SetOnChanged(std::function<void()> callback);
	void SyncProcCurrentDir();
private:
	std::filesystem::path cd;
	std::function<void()> callback;
};

