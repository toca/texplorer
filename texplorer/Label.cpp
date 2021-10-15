#include "Label.h"
#include "Screen.h"
#include "Cell.h"

tuindow::Label::Label(const std::wstring& text)
    : text(text)
    , style(Style::Default())
{
}

tuindow::Label::Label(const std::wstring& text, Placement placement)
    : text(text)
    , placement(placement)
    , style(Style::Default())
{
}


tuindow::Label::Label(const std::wstring& text, std::shared_ptr<Style> style)
    : text(text)
    , style(style)
{
}

tuindow::Label::Label(const std::wstring& text, Placement placement, std::shared_ptr<Style> style)
    : text(text)
    , placement(placement)
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
    // Вс
    for (uint32_t x = this->rect.left; long(x) < this->rect.right;)
    {
        if (i < this->text.size())
        {
            x += this->screen->Put(Cell(this->text[i++], this->style), x, y);
        }
        else
        {
            x += this->screen->Put(Cell(L' ', this->style), x, y);
        }
    }
    this->updated = false;
}

void tuindow::Label::RefreshImpl()
{
    this->updated = true;
}

void tuindow::Label::SetRectImpl(RECT rect)
{
    this->rect = rect;
    this->updated = true;
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
    return tuindow::FixedWidth(this->placement);
}

uint16_t tuindow::Label::FixedHeightImpl()
{
    return tuindow::FixedHeight(this->placement);
}

uint32_t tuindow::Label::WidthRatioImpl()
{
    return tuindow::WidthRatio(this->placement);
}

uint32_t tuindow::Label::HeightRatioImpl()
{
    return tuindow::HeightRatio(this->placement);
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

void tuindow::Label::SetStyle(std::shared_ptr<Style> style)
{
    this->style = style;
    this->updated = true;
}
