#include "Label.h"
#include "Screen.h"
#include "Character.h"


tuindow::Label::Label(const std::wstring& text)
    :text(text)
    ,rect({0,0,0,0})
    ,screen(nullptr)
{
}

void tuindow::Label::RenderImpl()
{
    uint32_t y = this->rect.top;
    size_t i = 0;
    for (uint32_t x = this->rect.left; long(x) < this->rect.right; x++)
    {
        if (i < this->text.size())
        {
            this->screen->Put(Character(this->text[i++]), x, y);
        }
        else
        {
            this->screen->Put(Character(L' '), x, y);
        }
    }
}

void tuindow::Label::SetRectImpl(RECT rect)
{
    this->rect = rect;
}

RECT tuindow::Label::GetRectImpl()
{
    return this->rect;
}

void tuindow::Label::SetScreenImpl(Screen* screen)
{
    this->screen = screen;
}

void tuindow::Label::Set(const std::wstring& text)
{
    this->text = text;
}

std::wstring tuindow::Label::Get()
{
    return std::wstring();
}
