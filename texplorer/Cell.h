#pragma once
#include <string>
#include <memory>

namespace tuindow
{
	enum class Color
	{
		BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, NONE
	};

	class Style 
	{
	public:
		Style();
		Style(const Color& fgColor, const Color& bgColor);
		static std::shared_ptr<Style> Default();
		std::shared_ptr<Style> Background(Color color);
		std::shared_ptr<Style> Foreground(Color color);
		const std::wstring PreSequence();
		const std::wstring PostSequence();
		bool operator==(const Style& lhs);
	private:
		void Init(const Color& fg, const Color& bg);
		Color fg;
		Color bg;
		std::wstring pre;

	};

	
	




	class Cell
	{
	public:
		Cell();
		Cell(wchar_t chara);
		Cell(wchar_t chara, std::shared_ptr<Style> style);
		wchar_t GetChar() const;
		std::wstring GetText() const;
		std::shared_ptr<Style> Style() const;
	private:
		wchar_t chara;
		std::shared_ptr<tuindow::Style> style;
	};
}

