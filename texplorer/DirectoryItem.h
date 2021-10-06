#pragma once
#include "SelectableList.h"
#include <filesystem>
namespace tuindow
{
    class Style;
}

class DirectoryItem :
    public tuindow::SelectableListItem
{
public:
    DirectoryItem(const std::filesystem::directory_entry& entry);

    std::wstring toString() override;
    virtual std::shared_ptr<tuindow::Style> toStyle() override;
private:
    std::filesystem::directory_entry entry;
};

