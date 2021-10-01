#include "Character.h"

namespace tuindow
{
    CharacterStyle::CharacterStyle(ForegroundColor fgColor, BackgroundColor bgColor)
        :fg(fgColor), bg(bgColor)
    {

    }

    CharacterStyle CharacterStyle::Default()
    {
        static CharacterStyle def(ForegroundColor::WHITE, BackgroundColor::BLACK);
        return def;
    }




    Character::Character()
        : chara(L' '), style(CharacterStyle::Default())
    {
    }

    Character::Character(wchar_t chara)
        : chara(chara), style(CharacterStyle::Default())
    {
    }

    Character::Character(wchar_t chara, CharacterStyle style)
        : chara(chara), style(style)
    {
    }

    wchar_t Character::Get()
    {
        return this->chara;
    }
}