#include "CurrentDir.h"
#include <filesystem>

//test
#include <Windows.h>

void CurrentDir::Change(const std::filesystem::path cd)
{
	if (!std::filesystem::is_directory(cd))
	{
		::OutputDebugString(L"Not a directory\n");
		return;
	}
	this->cd = std::filesystem::absolute(cd);
	if (this->callback)
	{
		this->callback();
	}
}

std::filesystem::path CurrentDir::Absolute()
{
	return this->cd;
}

std::vector<std::filesystem::directory_entry> CurrentDir::GetItems()
{
	std::vector<std::filesystem::directory_entry> result;
	for (auto entry : std::filesystem::directory_iterator(cd))
	{
		result.push_back(entry);
		::OutputDebugString(entry.path().filename().c_str());
		::OutputDebugString(L"\n");
	}
	return result;
}

void CurrentDir::SetOnChanged(std::function<void()> callback)
{
	this->callback = callback;
}
