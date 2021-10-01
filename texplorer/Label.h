#pragma once
#include <Windows.h>
#include <string>
#include "Widget.h"

namespace tuindow
{
	class Screen;

	class Label : public Widget
	{
	public:
		Label(const std::wstring& text);

		void RenderImpl() override;
		void SetRectImpl(RECT rect) override;
		RECT GetRectImpl() override;
		void SetScreenImpl(Screen* screen) override;

		void Set(const std::wstring& text);
		std::wstring Get();

	private:
		std::wstring text;
		RECT rect;
		Screen* screen;
	};

}
