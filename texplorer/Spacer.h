#pragma once
#include "Widget.h"
#include <memory>

namespace tuindow
{
	class Screen;
	class Style;

	class Spacer :
		public Widget
	{
	public:
		Spacer(Placement placement, std::shared_ptr<Style> style);
		void RenderImpl();
		void RefreshImpl();
		void SetRectImpl(RECT rect);
		RECT GetRectImpl();
		void SetScreenImpl(Screen* screen);
		uint16_t FixedWidthImpl();
		uint16_t FixedHeightImpl();
		uint32_t WidthRatioImpl();
		uint32_t HeightRatioImpl();
	private:
		Placement placement;
		std::shared_ptr<Style> style;
		RECT rect = { 0,0,0,0 };
		Screen* screen = nullptr;
		bool updated = true;
	};

}
