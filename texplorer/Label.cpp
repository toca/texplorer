#include "Label.h"
#include "Screen.h"
#include "Cell.h"

tuindow::Label::Label(const std::wstring& text)
    : text(text)
    , style(Style::Default())
{
}

tuindow::Label::Label(const std::wstring& text, FixedSize size)
    : text(text)
    , size(size)
    , style(Style::Default())
{
}

tuindow::Label::Label(const std::wstring& text, Ratio ratio)
    : text(text)
    , ratio(ratio)
    , style(Style::Default())
{
}


tuindow::Label::Label(const std::wstring& text, std::shared_ptr<Style> style)
    : text(text)
    , style(style)
{
}

tuindow::Label::Label(const std::wstring& text, FixedSize size, std::shared_ptr<Style> style)
    : text(text)
    , size(size)
    , style(style)
{
}

tuindow::Label::Label(const std::wstring& text, Ratio ratio, std::shared_ptr<Style> style)
    : text(text)
    , ratio(ratio)
    , style(style)
{
}

void tuindow::Label::RenderImpl()
{
    if (!this->updated)
    {
        return;
    }
    uint32_t y = this->rect.top;
    size_t i = 0;
    for (uint32_t x = this->rect.left; long(x) < this->rect.right; x++)
    {
        if (i < this->text.size())
        {
            this->screen->Put(Cell(this->text[i++], this->style), x, y);
        }
        else
        {
            this->screen->Put(Cell(L' ', this->style), x, y);
        }
    }
    this->updated = false;
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

uint16_t tuindow::Label::FixedWidthImpl()
{
    return this->size.width;
}

uint16_t tuindow::Label::FixedHeightImpl()
{
    return this->size.height;
}

uint32_t tuindow::Label::WidthRatioImpl()
{
    return this->ratio.h;
}

uint32_t tuindow::Label::HeightRatioImpl()
{
    return this->ratio.v;
}

void tuindow::Label::Set(const std::wstring& text)
{
    this->text = text;
    this->updated = true;
}

std::wstring tuindow::Label::Get()
{
    return std::wstring();
}
