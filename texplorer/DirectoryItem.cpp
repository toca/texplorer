#include "DirectoryItem.h"
#include "Cell.h"
#include <filesystem>

DirectoryItem::DirectoryItem(const std::filesystem::directory_entry& entry)
    : entry(entry)
{
}

std::wstring DirectoryItem::toString()
{
    if (this->entry.is_directory())
    {
        return this->entry.path().filename().wstring() + L"\\";
    }
    else
    {
        return this->entry.path().filename().wstring();
    }
}

std::shared_ptr<tuindow::Style> DirectoryItem::toStyle()
{
    auto static dirStyle = tuindow::Style::Default()->Foreground(tuindow::Color::CYAN);
    auto static linkStyle = tuindow::Style::Default()->Foreground(tuindow::Color::BLUE);
    if (this->entry._Is_symlink_or_junction())
    {
        return linkStyle;
    }
    if (this->entry.is_directory())
    {
        return dirStyle;
    }
        
    return tuindow::Style::Default();
}
