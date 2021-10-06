#include "DirectoryItemSize.h"
#include "Cell.h"

DirectoryItemSize::DirectoryItemSize(const std::filesystem::directory_entry& entry)
	: entry(entry)
{
}

std::wstring DirectoryItemSize::toString()
{
	//auto st = this->entry.status();
	//auto ft = st.type();
	//auto fp = st.permissions();

	if (this->entry.is_directory())
	{
		return L"-----";
	}
	std::wstring unit[5] = { L" B", L" K", L" M", L" G", L" T"};
	double size = this->entry.file_size();
	for (int i = 0; i < 5; i++)
	{
		if (size < 1000)
		{
			return std::to_wstring(size).substr(0,3) + unit[i];
		}
		size = size / 1024;
	}
	return std::to_wstring(this->entry.file_size()) + L"B";
}

std::shared_ptr<tuindow::Style> DirectoryItemSize::toStyle()
{
	return tuindow::Style::Default();
}
