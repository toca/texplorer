#pragma once
#include "SelectableList.h"
#include <filesystem>
namespace tuindow
{
	class Style;
}

class DirectoryItemDate :
	public tuindow::SelectableListItem
{
public:
	DirectoryItemDate(const std::filesystem::directory_entry& entry);

	std::wstring toString() override;
	virtual std::shared_ptr<tuindow::Style> toStyle() override;
private:
	std::filesystem::directory_entry entry;
	std::wstring Timestamp(const std::wstring& fmt = L"%F %H:%M:%S");
};
