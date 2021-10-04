#include "HorizontalBox.h"

void tuindow::HorizontalBox::RenderImpl()
{
	for (auto& child : this->children)
	{
		child->Render();
	}
}

void tuindow::HorizontalBox::SetRectImpl(RECT rect)
{
	this->rect = rect;
	this->Arrange();
}

RECT tuindow::HorizontalBox::GetRectImpl()
{
	return this->rect;
}

void tuindow::HorizontalBox::SetScreenImpl(Screen* screen)
{
	this->screen = screen;
	for (auto child : this->children)
	{
		child->SetScreen(screen);
	}
}

uint16_t tuindow::HorizontalBox::FixedWidthImpl()
{
	return uint16_t();
}

uint16_t tuindow::HorizontalBox::FixedHeightImpl()
{
	return uint16_t();
}

uint32_t tuindow::HorizontalBox::WidthRatioImpl()
{
	return uint32_t();
}

uint32_t tuindow::HorizontalBox::HeightRatioImpl()
{
	return uint32_t();
}

void tuindow::HorizontalBox::Push(std::shared_ptr<Widget> child)
{
	this->children.push_back(child);
	child->SetScreen(this->screen);
	this->Arrange();
}

void tuindow::HorizontalBox::Arrange()
{
	auto width = this->rect.right - this->rect.left;

	uint32_t totalRatio = 0;
	for (auto child : this->children)
	{
		auto fixedWidth = child->FixedWidth();
		width -= fixedWidth;
		if (!fixedWidth)
		{
			totalRatio += child->WidthRatio();
		}
	}
	width = max(width, 0);
	/*    LONG    left;
	LONG    top;
	LONG    right;
	LONG    bottom;*/
	long offset = 0;
	for (auto child : this->children)
	{
		long childWidth = child->FixedWidth()
			? child->FixedWidth()
			: width / totalRatio * child->WidthRatio();
		child->SetRect({
			this->rect.left + offset,
			this->rect.top,
			this->rect.left + offset + childWidth,
			this->rect.bottom
		});
		offset += childWidth;
	}

}
