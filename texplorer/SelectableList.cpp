#include "SelectableList.h"
#include "Label.h"
#include "Screen.h"

namespace tuindow
{
	SelectableList::SelectableList(Placement placement)
		: placement(placement)
	{
	}


	void SelectableList::RenderImpl()
	{
		if (!this->updated) return;
		this->updated = false;

		static auto inverted = std::make_shared<Style>(Color::BLACK, Color::WHITE);
		for (int i = 0; i < this->list.size() && i < this->numDisplay; i++)
		{
			auto itemIndex = i + this->displayBegin;
			this->labels[i]->Set(this->list[itemIndex]->toString());
			if (itemIndex == this->selected)
			{
				this->labels[i]->SetStyle(inverted);
			}
			else 
			{
				this->labels[i]->SetStyle(this->list[itemIndex]->toStyle());
			}
			this->labels[i]->Render();
		}
	}

	void SelectableList::SetRectImpl(RECT rect)
	{
		this->rect = rect;
		this->Arrange();
	}

	RECT SelectableList::GetRectImpl()
	{
		return this->rect;
	}
	void SelectableList::SetScreenImpl(Screen* screen)
	{
		this->screen = screen;
		for (auto label : this->labels)
		{
			label->SetScreen(screen);
		}
	}
	uint16_t SelectableList::FixedWidthImpl()
	{
		return tuindow::FixedWidth(this->placement);
	}
	uint16_t SelectableList::FixedHeightImpl()
	{
		return tuindow::FixedHeight(this->placement);
	}
	uint32_t SelectableList::WidthRatioImpl()
	{
		return tuindow::WidthRatio(this->placement);
	}
	uint32_t SelectableList::HeightRatioImpl()
	{
		return tuindow::HeightRatio(this->placement);
	}
	void SelectableList::Set(std::vector<std::shared_ptr<SelectableListItem>> list)
	{
		this->list = list;
		this->Arrange();
	}
	void SelectableList::Select(int index)
	{
		this->selected = index;
		this->updated = true;
	}
	int SelectableList::Selected()
	{
		return this->selected;
	}

	void SelectableList::Up()
	{
		this->updated = true;
		if (int64_t(this->selected) - 1 < 0)
		{
			this->selected = this->list.size() - 1;
			this->displayBegin = max(0, this->selected+1 - this->numDisplay);
		}
		else
		{
			if (this->selected - 1 < this->displayBegin)
			{
				this->displayBegin--;
			}
			this->selected--;
		}
	}

	void SelectableList::Down()
	{
		this->updated = true;
		if (int64_t(this->list.size()) <= this->selected + 1)
		{
			this->selected = 0;
			this->displayBegin = 0;
		}
		else
		{
			if (this->displayBegin + this->numDisplay - 1 < this->selected + 1)
			{
				this->displayBegin++;
			}
			this->selected++;
		}
	}

	void SelectableList::Arrange()
	{
		this->updated = true;

		this->numDisplay = int64_t(this->rect.bottom - this->rect.top);
		this->displayBegin = 0;
		
		this->Clear();
		this->labels.clear();
		int row = 0;
		for (int row = 0; row < this->numDisplay; row++)
		{
			auto label = std::make_shared<Label>(L"");
			label->SetRect({
				this->rect.left,
				this->rect.top + row,
				this->rect.right,
				this->rect.top + row + 1
			});
			label->SetScreen(this->screen);
			this->labels.push_back(label);
		}
	}
	void SelectableList::Clear()
	{
		if (!this->screen)
		{
			return;
		}
		Cell empty(L' ');
		for (auto y = this->rect.top; y < this->rect.bottom; y++)
		{
			for (auto x = this->rect.left; x < this->rect.right; x++)
			{
				this->screen->Put(empty, x, y);
			}
		}
	}
}
