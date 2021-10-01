#include "Address.h"

std::wstring Address::Get()
{
	return this->data;
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
