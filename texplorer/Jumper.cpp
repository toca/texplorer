#include "Jumper.h"
#include <stdexcept>

std::wstring Jumper::Message()
{
    if (this->crouching)
    {
        return L"Input first letter of filename to select item";
    }
    else
    {
        return L"GoTo[^G]";
    }
}

void Jumper::Ready()
{
    this->crouching = true;
    if (this->callback) this->callback();
}

size_t Jumper::Find(wchar_t initial, const std::vector<std::filesystem::directory_entry>& entries)
{
    this->crouching = false;
    if (this->callback) this->callback();

    std::wstring str(1, std::tolower(initial));
    for (size_t i = 0; i < entries.size(); i++)
    {
        std::wstring name = entries[i].path().filename().wstring();
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (name.starts_with(str))
        {
            return i;
        }
    }
    throw std::exception("Not found");
}

bool Jumper::Crouching()
{
    return this->crouching;
}

void Jumper::AddSubscriber(std::function<void()> callback)
{
    this->callback = callback;
}
