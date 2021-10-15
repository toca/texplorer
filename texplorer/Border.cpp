#include "Border.h"
#include "Screen.h"
#include "Cell.h"
#include "Label.h"
#include "CharacterWidth.h"

namespace tuindow
{
	Border::Border(std::wstring left, std::wstring top, std::wstring right, std::wstring bottom, std::wstring topLeft, std::wstring topRight, std::wstring bottomLeft, std::wstring bottomRight)
		: left(left)
		, top(top)
		, right(right)
		, bottom(bottom)
		, topLeft(topLeft)
		, topRight(topRight)
		, bottomLeft(bottomLeft)
		, bottomRight(bottomRight)
	{
	}
	void Border::RenderImpl()
	{
		for (int i = 0; i < this->borders.size(); i++)
		{
			this->borders[i]->Render();
		}
		child->Render();
	}

	void Border::RefreshImpl()
	{
		this->child->Refresh();
	}

	void Border::SetRectImpl(RECT rect)
	{
		this->rect = rect;
		this->Arrange();
	}

	RECT Border::GetRectImpl()
	{
		return this->rect;
	}

	void Border::SetScreenImpl(Screen* screen)
	{
		this->screen = screen;
		child->SetScreen(screen);
		for (int i = 0; i < this->borders.size(); i++)
		{
			this->borders[i]->SetScreen(screen);
		}
	}

	uint16_t Border::FixedWidthImpl()
	{
		return tuindow::FixedWidth(this->placement);
	}

	uint16_t Border::FixedHeightImpl()
	{
		return tuindow::FixedWidth(this->placement);
	}

	uint32_t Border::WidthRatioImpl()
	{
		return tuindow::WidthRatio(this->placement);
	}

	uint32_t Border::HeightRatioImpl()
	{
		return tuindow::HeightRatio(this->placement);
	}

	void Border::Put(std::shared_ptr<Widget> child)
	{
		this->child = child;
		this->Arrange();
	}

	void Border::Arrange()
	{
		// TODO calc propery
		/*int topLen = this->top.empty()   ? 0 : Util::CharacterWidth(this->top.back());
		int botLen = this->bottom.empty()  ? 0 : Util::CharacterWidth(this->bottom.back());
		int rightLen = this->right.empty() ? 0 : Util::CharacterWidth(this->right.back());
		int leftLen = this->left.empty()   ? 0 : Util::CharacterWidth(this->left.back());*/

		const int padding = 1;
		child->SetRect({
			this->rect.left + (this->left.empty() ? 0 : Util::CharacterWidth(this->left.front()) + padding),
			this->rect.top + (this->top.empty() ? 0 : 1),
			this->rect.right - (this->right.empty() ? 0 : Util::CharacterWidth(this->right.front()) + padding),
			this->rect.bottom - (this->bottom.empty() ?  0 : 1)
		});
		
		this->borders.clear();
		// left, right
		for (int i = (top.empty() ? 0 : 1); i < (this->rect.bottom - this->rect.top - (bottom.empty() ? 0 : 1)); i++)
		{
			if (!this->left.empty())
			{
				auto border = std::make_shared<Label>(this->left + L" ");
				border->SetRect({
					this->rect.left,
					this->rect.top + i,
					this->rect.left + padding + Util::CharacterWidth(this->left.front()),
					this->rect.top + i + 1
				});
				border->SetScreen(this->screen);
				this->borders.push_back(border);
			}

			if (!this->right.empty())
			{
				auto border = std::make_shared<Label>(L" " + this->right);
				border->SetRect({
					this->rect.right - Util::CharacterWidth(this->right.front()) - padding,
					this->rect.top + i,
					this->rect.right,
					this->rect.top + i + 1
					});
				border->SetScreen(this->screen);
				this->borders.push_back(border);
			}
		}
		// top, bottom
		for (
			int i = (left.empty() ? 1 : Util::CharacterWidth(this->left.front()));
			i < (this->rect.right - this->rect.left - (right.empty() ? 1 : Util::CharacterWidth(this->right.front())));
			i++)
		{
			if (!this->top.empty())
			{
				auto border = std::make_shared<Label>(this->top);
				border->SetRect({
					this->rect.left + i,
					this->rect.top,
					this->rect.left + i + Util::CharacterWidth(this->top.front()),
					this->rect.top + 1
				});
				border->SetScreen(this->screen);
				this->borders.push_back(border);
			}

			if (!this->bottom.empty())
			{
				auto border = std::make_shared<Label>(this->bottom);
				border->SetRect({
					this->rect.left + i ,
					this->rect.bottom - 1,
					this->rect.left + i + Util::CharacterWidth(this->bottom.front()),
					this->rect.bottom
					});
				border->SetScreen(this->screen);
				this->borders.push_back(border);
			}
		}
		// corner
		if (!this->bottomLeft.empty())
		{
			auto border = std::make_shared<Label>(this->bottomLeft);
			border->SetRect({
				this->rect.left,
				this->rect.bottom - 1,
				this->rect.left + Util::CharacterWidth(this->bottomLeft.front()),
				this->rect.bottom
				});
			border->SetScreen(this->screen);
			this->borders.push_back(border);
		}
		if (!this->bottomRight.empty())
		{
			auto border = std::make_shared<Label>(this->bottomRight);
			border->SetRect({
				this->rect.right - Util::CharacterWidth(this->right.front()),
				this->rect.bottom - 1,
				this->rect.right,
				this->rect.bottom
				});
			border->SetScreen(this->screen);
			this->borders.push_back(border);
		}
	}




}
