#pragma once
#include "Widget.h"
#include <vector>
#include <memory>
#include <string>

namespace tuindow
{
    class Screen;
    class Label;

    class Border :
        public Widget
    {
    public:
        Border(std::wstring left, std::wstring top, std::wstring right, std::wstring bottom, std::wstring topLeft, std::wstring topRight, std::wstring bottomLeft, std::wstring bottomRight);
        void RenderImpl() override;
        void RefreshImpl() override;
        void SetRectImpl(RECT rect) override;
        RECT GetRectImpl() override;
        void SetScreenImpl(Screen* screen) override;

        uint16_t FixedWidthImpl() override;
        uint16_t FixedHeightImpl() override;
        uint32_t WidthRatioImpl() override;
        uint32_t HeightRatioImpl() override;

        void Put(std::shared_ptr<Widget> child);
    private:
        void Arrange();
        Screen* screen = nullptr; 
        std::shared_ptr<Widget> child;
        std::vector<std::shared_ptr<Label>> borders;
        RECT rect = {};
        Placement placement = { 1, 1, false, false };
        std::wstring top;
        std::wstring bottom;
        std::wstring left;
        std::wstring right;
        std::wstring topLeft;
        std::wstring topRight;
        std::wstring bottomLeft;
        std::wstring bottomRight;
    };
}
