#pragma once
#include <Windows.h>

namespace tuindow
{
	class Screen;

	class Widget
	{
	public:
		virtual ~Widget() = default;

		void Render()
		{
			this->RenderImpl();
		}
		void SetRect(RECT rect)
		{
			this->SetRectImpl(rect);
		}
		RECT GetRect()
		{
			this->GetRectImpl();
		}
		void SetScreen(Screen* screen)
		{
			this->SetScreenImpl(screen);
		}

	protected:
		Widget() = default;
		virtual void RenderImpl() = 0;
		virtual void SetRectImpl(RECT rect) = 0;
		virtual RECT GetRectImpl() = 0;
		virtual void SetScreenImpl(Screen* screen) = 0;
	private:
		Widget(const Widget&) = delete;
		Widget(Widget&&) = delete;
		Widget& operator =(const Widget&) = delete;
		Widget& operator =(Widget&&) = delete;
	};
}

