#pragma once
#include <Windows.h>
#include <cstdint>

namespace tuindow
{
	class Screen;

	struct Placement
	{
		uint32_t width;
		uint32_t height;
		bool widthFixed;
		bool heightFixed;
	};

	uint32_t FixedWidth(const Placement& p);
	uint32_t FixedHeight(const Placement& p);
	uint32_t WidthRatio(const Placement& p);
	uint32_t HeightRatio(const Placement& p);

	class Widget
	{
	public:
		virtual ~Widget() = default;

		void Render()
		{
			this->RenderImpl();
		}
		void Refresh()
		{
			this->RefreshImpl();
		}
		void SetRect(RECT rect)
		{
			this->SetRectImpl(rect);
		}
		RECT GetRect()
		{
			return this->GetRectImpl();
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
		virtual void RefreshImpl() = 0;
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

