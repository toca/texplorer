#pragma once
#include "Widget.h"
#include <memory>
#include <vector>

namespace tuindow
{
    class VerticalBox : public Widget
    {
     public:
        VerticalBox(Placement place);
        VerticalBox();
        void RenderImpl() override;
        void RefreshImpl() override;
        void SetRectImpl(RECT rect) override;
        RECT GetRectImpl() override;
        void SetScreenImpl(Screen* screen) override;

        uint16_t FixedWidthImpl() override;
        uint16_t FixedHeightImpl() override;
        uint32_t WidthRatioImpl() override;
        uint32_t HeightRatioImpl() override;

        void Push(std::shared_ptr<Widget> child);
    private:
        void Arrange();
        std::vector<std::shared_ptr<Widget>> children;
        RECT rect = { 0, 0, 0, 0 };
        Screen* screen = nullptr;
        Placement placement = { 1, 1, false, false };
    };
}
