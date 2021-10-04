#pragma once
#include <Windows.h>
#include <cstdint>

namespace tuindow
{
	class Screen;

	struct FixedSize
	{
		uint16_t width;
		uint16_t height;
	};

	struct Ratio
	{
		uint32_t h;
		uint32_t v;
	};

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
		uint16_t FixedWidth() 
		{
			return this->FixedWidthImpl();
		}
		uint16_t FixedHeight()
		{
			return this->FixedHeightImpl();
		}
		uint32_t WidthRatio()
		{
			return this->WidthRatioImpl();
		}
		uint32_t HeightRatio()
		{
			return this->HeightRatioImpl();
		}

	protected:
		Widget() = default;
		virtual void RenderImpl() = 0;
		virtual void SetRectImpl(RECT rect) = 0;
		virtual RECT GetRectImpl() = 0;
		virtual void SetScreenImpl(Screen* screen) = 0;
		virtual uint16_t FixedWidthImpl() = 0;
		virtual uint16_t FixedHeightImpl() = 0;
		virtual uint32_t WidthRatioImpl() = 0;
		virtual uint32_t HeightRatioImpl() = 0;

	private:
		Widget(const Widget&) = delete;
		Widget(Widget&&) = delete;
		Widget& operator =(const Widget&) = delete;
		Widget& operator =(Widget&&) = delete;
	};
}

