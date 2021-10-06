#pragma once
#include "Widget.h"
#include <memory>
#include <vector>
#include <string>

namespace tuindow 
{
	class Label;
	class Style;

	class SelectableListItem
	{
	public:
		SelectableListItem() = default;
		virtual ~SelectableListItem() = default;
		virtual std::wstring toString() = 0;
		virtual std::shared_ptr<Style> toStyle()= 0;
	private:
		SelectableListItem(const SelectableListItem&) = delete;
		SelectableListItem(SelectableListItem&&) = delete;
		SelectableListItem& operator =(const SelectableListItem&) = delete;
		SelectableListItem& operator =(SelectableListItem&&) = delete;
	};

	class SelectableList : public Widget
	{
	public:
		SelectableList() = default;
		SelectableList(Placement placement);

		void RenderImpl() override;
		void SetRectImpl(RECT rect) override;
		RECT GetRectImpl() override;
		void SetScreenImpl(Screen* screen) override;
		uint16_t FixedWidthImpl() override;
		uint16_t FixedHeightImpl() override;
		uint32_t WidthRatioImpl() override;
		uint32_t HeightRatioImpl() override;

		void Set(std::vector<std::shared_ptr<SelectableListItem>> list);
		void Select(int index);
		int64_t Selected();
		void Up();
		void Down();

	private:
		void Arrange();
		void Clear();
		std::vector<std::shared_ptr<SelectableListItem>> list;
		std::vector<std::shared_ptr<Label>> labels;
		RECT rect = { 0, 0, 0, 0 };
		Screen* screen = nullptr;
		std::shared_ptr<Style> style;
		Placement placement = { 1, 1, false, false };
		int64_t selected = 0;
		bool updated = true;
		int64_t numDisplay = 0;
		int64_t displayBegin = 0;
	};
}
