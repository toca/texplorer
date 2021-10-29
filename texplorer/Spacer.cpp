#include "Spacer.h"
#include "Screen.h"
#include "Cell.h"

namespace tuindow
{
	Spacer::Spacer(Placement placement, std::shared_ptr<Style> style)
		: placement(placement)
		, style(style)
	{
	}
	void Spacer::RenderImpl()
	{
		if (!this->updated)
		{
			return;
		}
		uint32_t y = this->rect.top;
		size_t i = 0;
		for (uint32_t x = this->rect.left; long(x) < this->rect.right;)
		{
			x += this->screen->Put(Cell(L' ', this->style), x, y);
		}
		this->updated = false;
	}
	void Spacer::RefreshImpl()
	{
		this->updated = true;
	}
	void Spacer::SetRectImpl(RECT rect)
	{
		this->rect = rect;
		this->updated = true;
	}
	RECT Spacer::GetRectImpl()
	{
		return this->rect;
	}
	void Spacer::SetScreenImpl(Screen* screen)
	{
		this->screen = screen;
	}
	uint16_t Spacer::FixedWidthImpl()
	{
		return tuindow::FixedWidth(this->placement);
	}
	uint16_t Spacer::FixedHeightImpl()
	{
		return tuindow::FixedHeight(this->placement);
	}
	uint32_t Spacer::WidthRatioImpl()
	{
		return tuindow::WidthRatio(this->placement);
	}
	uint32_t Spacer::HeightRatioImpl()
	{
		return tuindow::HeightRatio(this->placement);
	}
}

