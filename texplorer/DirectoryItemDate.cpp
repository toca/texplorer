#include "DirectoryItemDate.h"
#include <filesystem>
#include <chrono>
#include "Cell.h"

DirectoryItemDate::DirectoryItemDate(const std::filesystem::directory_entry& entry)
	: entry(entry)
{
}

std::wstring DirectoryItemDate::toString()
{
	return this->Timestamp();
}

std::shared_ptr<tuindow::Style> DirectoryItemDate::toStyle()
{
	return tuindow::Style::Default();
}

std::wstring DirectoryItemDate::Timestamp(const std::wstring& fmt)
{
	static const auto EpochTimeBias = std::chrono::system_clock::now().time_since_epoch() - std::filesystem::file_time_type::clock::now().time_since_epoch();

	auto lastWrite = this->entry.last_write_time();
	auto sec = std::chrono::duration_cast<std::chrono::seconds>(lastWrite.time_since_epoch() + EpochTimeBias);
	std::time_t t = sec.count();
	struct tm time;
	localtime_s(&time, &t);
	std::wstringstream ss;
	ss << std::put_time(&time, fmt.c_str());
	return ss.str();
}
