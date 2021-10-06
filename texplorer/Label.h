#pragma once
#include <Windows.h>
#include <string>
#include <memory>
#include "Widget.h"
#include "Cell.h"

namespace tuindow
{
	class Screen;

	class Label : public Widget
	{
	public:
		explicit Label(const std::wstring& text);
		explicit Label(const std::wstring& text, Placement placement);
		explicit Label(const std::wstring& text, std::shared_ptr<Style> style);
		explicit Label(const std::wstring& text, Placement placement, std::shared_ptr<Style> style);

		void RenderImpl() override;
		void SetRectImpl(RECT rect) override;
		RECT GetRectImpl() override;
		void SetScreenImpl(Screen* screen) override;
		uint16_t FixedWidthImpl() override;
		uint16_t FixedHeightImpl() override;
		uint32_t WidthRatioImpl() override;
		uint32_t HeightRatioImpl() override;

		void Set(const std::wstring& text);
		std::wstring Get();
		void SetStyle(std::shared_ptr<Style> style);

	private:
		std::wstring text;
		RECT rect = { 0, 0, 0, 0 };
		Screen* screen = nullptr;
		std::shared_ptr<Style> style;
		Placement placement = { 1, 1, false, false };
		bool updated = true;
	};

}
