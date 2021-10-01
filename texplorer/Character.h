#pragma once
namespace tuindow
{
	enum class ForegroundColor 
	{
		WHITE, BLACK
	};
	enum class BackgroundColor
	{
		BLACK, WHITE
	};

	class CharacterStyle 
	{
	public:
		CharacterStyle(ForegroundColor fgColor, BackgroundColor bgColor);
		static CharacterStyle Default();
	private:
		ForegroundColor fg;
		BackgroundColor bg;

	};

	
	




	class Character
	{
	public:
		Character();
		Character(wchar_t chara);
		Character(wchar_t chara, CharacterStyle style);
		wchar_t Get();
	private:
		wchar_t chara;
		CharacterStyle style;
	};
}

