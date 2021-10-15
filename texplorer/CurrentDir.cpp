#include "CurrentDir.h"
#include <filesystem>
#include <algorithm>
#include <Windows.h>

#include "CharacterWidth.h"

void CurrentDir::Change(const std::filesystem::path cd)
{
	if (!std::filesystem::is_directory(cd))
	{
		::OutputDebugString(L"Not a directory\n");
		throw std::runtime_error(tuindow::Util::format("directory '%s' not exists!", cd.filename().string().c_str()).c_str());
	}
	try 
	{
		auto it = std::filesystem::directory_iterator(cd);
	}
	catch (std::exception& ex)
	{
		::OutputDebugString(L"Permission denied\n");
		return;
	}
	this->cd = std::filesystem::absolute(cd);
	if (this->callback)
	{
		this->callback();
	}
}

void CurrentDir::Up()
{
	this->Change(this->cd.parent_path());
}

void CurrentDir::Into(int index)
{
	auto items = this->GetItems();
	auto selected = items.at(index);
	if (selected.is_directory())
	{
		this->Change(selected.path());
	}
}


std::filesystem::path CurrentDir::Absolute()
{
	return this->cd;
}

std::vector<std::filesystem::directory_entry> CurrentDir::GetItems()
{
	std::vector<std::filesystem::directory_entry> result;
	for (auto entry : std::filesystem::directory_iterator(this->cd))
	{
		result.push_back(entry);
		//::OutputDebugString(entry.path().filename().c_str());
		//::OutputDebugString(L"\n");
	}

	auto order = this->asc;
	switch (this->orderBy)
	{
	case OrderBy::NAME:
		std::sort(result.begin(), result.end(), [order](const std::filesystem::directory_entry& lhs, const std::filesystem::directory_entry& rhs) {
			if (order) {
				return lhs.path().filename() < rhs.path().filename();
			}
			else {
				return lhs.path().filename() > rhs.path().filename();
			}
		});
		break;
	case OrderBy::DATE:
		std::sort(result.begin(), result.end(), [order](const std::filesystem::directory_entry& lhs, const std::filesystem::directory_entry& rhs) {
			if (order) {
				return lhs.last_write_time() < rhs.last_write_time();
			}
			else {
				return lhs.last_write_time() > rhs.last_write_time();
			}
		});
		break;
	case OrderBy::SIZE:
		std::sort(result.begin(), result.end(), [order](const std::filesystem::directory_entry& lhs, const std::filesystem::directory_entry& rhs) {
			if (order) {
				return lhs.file_size() < rhs.file_size();
			}
			else {
				return lhs.file_size() > rhs.file_size();
			}
		});
		break;
	}
	return result;
}

std::filesystem::path CurrentDir::Get(int index)
{
	return this->GetItems()[index].path();
}

void CurrentDir::SetOnChanged(std::function<void()> callback)
{
	this->callback = callback;
}

void CurrentDir::SortName()
{
	this->Sort(OrderBy::NAME);
}
void CurrentDir::SortDate()
{
	this->Sort(OrderBy::DATE);
}
void CurrentDir::SortSize()
{
	this->Sort(OrderBy::SIZE);
}

void CurrentDir::Sort(OrderBy by)
{
	this->asc = !this->asc;
	this->orderBy = by;
	if (this->callback)
	{
		this->callback();
	}
}

