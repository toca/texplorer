#include "Cell.h"
#include <windows.h>

namespace tuindow
{
    Style::Style()
    {
        this->Init(Color::WHITE, Color::BLACK);
    }

    Style::Style(const Color& fgColor, const Color& bgColor)
    {
        this->Init(fgColor, bgColor);
    }



    std::shared_ptr<Style> Style::Default()
    {
        static std::shared_ptr<Style> defaultStyle = std::make_shared<Style>(Color::WHITE, Color::BLACK);
        return defaultStyle;
    }

    std::shared_ptr<Style> Style::Background(Color color)
    {
        return std::make_shared<Style>(this->fg, color);
    }

    std::shared_ptr<Style> Style::Foreground(Color color)
    {
        return std::make_shared<Style>(color, this->bg);
    }

    const std::wstring Style::PreSequence()
    {
        return this->pre;
    }


    const std::wstring Style::PostSequence()
    {
        return L"\033[0m";
    }

    bool Style::operator==(const Style& lhs)
    {
        return this->bg == lhs.bg && this->fg == lhs.fg;

    }

    void Style::Init(const Color& fgColor, const Color& bgColor)
    {
        this->fg = fgColor;
        this->bg = bgColor;

        switch (fgColor)
        {
        case Color::BLACK:
            this->pre = L"\033[30m";
            break;
        case Color::RED:
            this->pre = L"\033[31m";
            break;
        case Color::GREEN:
            this->pre = L"\033[32m";
            break;
        case Color::YELLOW:
            this->pre = L"\033[33m";
            break;
        case Color::BLUE:
            this->pre = L"\033[34m";
            break;
        case Color::MAGENTA:
            this->pre = L"\033[35m";
            break;
        case Color::CYAN:
            this->pre = L"\033[36m";
            break;
        case Color::WHITE:
            this->pre = L"\033[37m";
            break;
        case Color::NONE:
            break;
        default:
            throw std::exception("unknown tuindow::Color");
        }
        switch (bgColor)
        {
        case Color::BLACK:
            this->pre +=  L"\033[40m";
            break;
        case Color::RED:
            this->pre +=  L"\033[41m";
            break;
        case Color::GREEN:
            this->pre +=  L"\033[42m";
            break;
        case Color::YELLOW:
            this->pre +=  L"\033[43m";
            break;
        case Color::BLUE:
            this->pre +=  L"\033[44m";
            break;
        case Color::MAGENTA:
            this->pre +=  L"\033[45m";
            break;
        case Color::CYAN:
            this->pre +=  L"\033[46m";
            break;
        case Color::WHITE:
            this->pre +=  L"\033[47m";
            break;
        case Color::NONE:
            break;
        default:
            throw std::exception("unknown tuindow::Color");
        }
    }






    Cell::Cell()
        : chara(L' '), style(Style::Default())
    {
    }

    Cell::Cell(wchar_t chara)
        : chara(chara), style(Style::Default())
    {
    }

    Cell::Cell(wchar_t chara, std::shared_ptr<tuindow::Style> style)
        : chara(chara), style(style)
    {
    }

    wchar_t Cell::GetChar() const
    {
        return this->chara;
    }

    std::wstring Cell::GetText() const
    {
        // fg sequence + bg sequence + character + terminate sequence
        // =     5     +     5       +     1     +      5
        wchar_t buf[16];
        wcscpy_s(buf, 16, this->style->PreSequence().c_str());
        buf[10] = this->chara;
        buf[11] = L'\0';
        wcscat_s(buf, 16, this->style->PostSequence().c_str());
        return std::wstring(buf);
    }

    std::shared_ptr<Style> Cell::Style() const
    {
        return this->style;
    }
}