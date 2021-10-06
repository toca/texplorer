#include "VerticalBox.h"

void tuindow::VerticalBox::RenderImpl()
{
	for (auto& child : this->children)
	{
		child->Render();
	}
}

void tuindow::VerticalBox::SetRectImpl(RECT rect)
{
	this->rect = rect;
	this->Arrange();
}

RECT tuindow::VerticalBox::GetRectImpl()
{
	return this->rect;
}

void tuindow::VerticalBox::SetScreenImpl(Screen* screen)
{
	this->screen = screen;
	for (auto child : this->children)
	{
		child->SetScreen(screen);
	}
}

uint16_t tuindow::VerticalBox::FixedWidthImpl()
{
	return tuindow::FixedWidth(this->placement);
}

uint16_t tuindow::VerticalBox::FixedHeightImpl()
{
	return tuindow::FixedHeight(this->placement);
}

uint32_t tuindow::VerticalBox::WidthRatioImpl()
{
	return tuindow::WidthRatio(this->placement);
}

uint32_t tuindow::VerticalBox::HeightRatioImpl()
{
	return tuindow::HeightRatio(this->placement);
}

void tuindow::VerticalBox::Push(std::shared_ptr<Widget> child)
{
	this->children.push_back(child);
	child->SetScreen(this->screen);
	this->Arrange();
}

void tuindow::VerticalBox::Arrange()
{
	auto height = this->rect.bottom - this->rect.top;

	uint32_t totalRatio = 0;
	for (auto child : this->children)
	{
		auto fixedHeight = child->FixedHeight();
		height -= fixedHeight;
		if (!fixedHeight)
		{
			totalRatio += child->HeightRatio();
		}
	}
	height = max(height, 0);
	/*    LONG    left;
	LONG    top;
	LONG    right;
	LONG    bottom;*/
	long offset = 0;
	for (auto child : this->children)
	{
		long childHeight = child->FixedHeight()
			? child->FixedHeight()
			: height / totalRatio * child->HeightRatio();
		child->SetRect({
			this->rect.left,
			this->rect.top + offset,
			this->rect.right,
			this->rect.top + offset + childHeight
			});
		offset += childHeight;
	}

}
