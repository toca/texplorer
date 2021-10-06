#include "Widget.h"

uint32_t tuindow::FixedWidth(const Placement& p)
{
	if (p.widthFixed)
	{
		return p.width;
	}
	return 0;
}

uint32_t tuindow::FixedHeight(const Placement& p)
{
	if (p.heightFixed)
	{
		return p.height;
	}
	return 0;
}

uint32_t tuindow::WidthRatio(const Placement& p)
{
	if (!p.widthFixed)
	{
		return p.width;
	}
	return 0;
}

uint32_t tuindow::HeightRatio(const Placement& p)
{
	if (!p.heightFixed)
	{
		return p.height;
	}
	return 0;
}

