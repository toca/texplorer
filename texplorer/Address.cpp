#include "Address.h"
#include <filesystem>

std::wstring Address::Get()
{
	return this->data;
}

void Address::Set(const std::wstring& addr)
{
	this->data = addr;
	this->OnChanged();
}

void Address::AddInput(const wchar_t& input)
{
	this->data += input;
	this->OnChanged();
}


void Address::Chop()
{
	if (!this->data.empty())
	{
		this->data.pop_back();
		this->OnChanged();
	}
}


void Address::SetOnChanged(std::function<void()> callback)
{
	this->callback = callback;
}

void Address::OnChanged()
{
	this->callback();
}

void Address::Complete()
{
	std::filesystem::path input(this->data);
	auto dir = input.parent_path();
	if (!std::filesystem::exists(dir))
	{
		return;
	}
	if (input == dir)
	{
		return;
	}
	auto const filename = input.filename().wstring();
	std::wstring incompleted;
	incompleted.resize(filename.size());
	std::transform(
		filename.begin(), filename.end(),
		incompleted.begin(),
		::tolower
	);
	bool found = false;
	for(auto const& entry : std::filesystem::directory_iterator(dir))
	{
		if (found)
		{
			this->Set(entry.path());
			return;
		}
		auto const filename = entry.path().filename().wstring();
		std::wstring name;
		name.resize(filename.size());
		std::transform(
			filename.begin(), filename.end(),
			name.begin(),
			::tolower
		);
		if (name == incompleted)
		{
			found = true;
		}
	}
	if (found)
	{
		auto const& entory = std::filesystem::directory_iterator(dir);
		this->Set(entory->path());
		return;
	}

	for (auto const& entry : std::filesystem::directory_iterator(dir))
	{
		auto const filename = entry.path().filename().wstring();
		std::wstring name;
		name.resize(filename.size());
		std::transform(
			filename.begin(), filename.end(),
			name.begin(),
			::tolower
		);
		if (name.starts_with(incompleted))
		{
			this->Set(entry.path());
			return;
		}
	}

}