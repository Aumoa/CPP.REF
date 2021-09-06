// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Widgets/Panel/CanvasPanel.h"
#include "Layout/ArrangedChildrens.h"
#include "Draw/PaintArgs.h"

SCanvasPanel::Slot::Slot()
{
}

SCanvasPanel::SCanvasPanel(const std::wstring& name) : Super(name)
{
}

SCanvasPanel::~SCanvasPanel()
{
}

DEFINE_SLATE_CONSTRUCTOR(SCanvasPanel, attr)
{
	Super::Construct(attr);
}

int32 SCanvasPanel::OnPaint(PaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const
{
	ArrangedChildrens arrangedChildren(ESlateVisibility::Visible);
	ArrangeChildren(&arrangedChildren, allottedGeometry);

	bool forwardedEnabled = ShouldBeEnabled(bParentEnabled);
	int maxLayer = layer;

	PaintArgs* newArgs = paintArgs->WithNewParent(this);
	for (auto& curWidget : arrangedChildren.GetWidgets())
	{
		if (!IsChildWidgetCulled(cullingRect, curWidget))
		{
			int curWidgetsMaxLayerId = curWidget.GetWidget()->Paint(
				newArgs,
				curWidget.GetGeometry(),
				cullingRect,
				drawElements,
				layer,
				forwardedEnabled);

			maxLayer = MathEx::Max(maxLayer, curWidgetsMaxLayerId);
		}
	}

	return maxLayer;
}

void SCanvasPanel::OnArrangeChildren(ArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const
{
	if (_childrens.size() > 0)
	{
		// Sort the children based on zorder.
		std::vector<ChildZOrder> slotOrder;
		slotOrder.reserve(_childrens.size());

		for (size_t i = 0; i < _childrens.size(); ++i)
		{
			auto& curChild = _childrens[i];
			slotOrder.emplace_back() =
			{
				.ChildIndex = (int32)i,
				.ZOrder = curChild->_ZOrder
			};
		}

		static auto compareAscending = [](auto lh, auto rh)
		{
			return lh == rh ? 0 : lh < rh ? -1 : 1;
		};

		static auto sortSlotsByZOrder = [](auto& lhs, auto& rhs)
		{
			return lhs.ZOrder == rhs.ZOrder
				? compareAscending(lhs.ChildIndex, rhs.ChildIndex)
				: compareAscending(lhs.ZOrder, rhs.ZOrder);
		};

		std::sort(slotOrder.begin(), slotOrder.end(), sortSlotsByZOrder);

		// Arrange the children now in their proper z-order.
		for (size_t childIndex = 0; childIndex < _childrens.size(); ++childIndex)
		{
			ChildZOrder curSlot = slotOrder[childIndex];
			Slot* curChild = _childrens[curSlot.ChildIndex];
			const SWidget* curWidget = curChild->GetContent();

			ESlateVisibility childVisibility = curWidget->GetVisibility();
			if (arrangedChildrens->Accepts(childVisibility))
			{
				Margin offset = curChild->_Offset;
				Vector2 alignment = curChild->_Alignment;
				Anchors anchors = curChild->_Anchors;

				bool bAutoSize = curChild->_bAutoSize;

				auto anchorPixels = Margin(
					anchors.Minimum.X * allottedGeometry.GetSize().X,
					anchors.Minimum.Y * allottedGeometry.GetSize().Y,
					anchors.Maximum.X * allottedGeometry.GetSize().X,
					anchors.Maximum.Y * allottedGeometry.GetSize().Y);

				bool bIsHorizontalStretch = anchors.Minimum.X != anchors.Maximum.X;
				bool bIsVerticalStretch = anchors.Minimum.Y != anchors.Maximum.Y;

				auto slotSize = Vector2(offset.Right, offset.Bottom);
				auto Size = bAutoSize ? curWidget->GetDesiredSize() : slotSize;

				// Calculate the offset based on the pivot position.
				Vector2 alignmentOffset = Size * alignment;

				// Calculate the local position based on the anchor and position offset.
				Vector2 localPosition, localSize;

				// Calculate the position and size based on the horizontal stretch or non-stretch
				if (bIsHorizontalStretch)
				{
					localPosition.X = anchorPixels.Left + offset.Left;
					localSize.X = anchorPixels.Right - localPosition.X - offset.Right;
				}
				else
				{
					localPosition.X = anchorPixels.Left + offset.Left - alignmentOffset.X;
					localSize.X = Size.X;
				}

				// Calculate the position and size based on the vertical stretch or non-stretch
				if (bIsVerticalStretch)
				{
					localPosition.Y = anchorPixels.Top + offset.Top;
					localSize.Y = anchorPixels.Bottom - localPosition.Y - offset.Bottom;
				}
				else
				{
					localPosition.Y = anchorPixels.Top + offset.Top - alignmentOffset.Y;
					localSize.Y = Size.Y;
				}

				// Add the information about this child to the output list (ArrangedChildren)
				arrangedChildrens->AddWidget(childVisibility, allottedGeometry.MakeChild(
					// The child widget being arranged
					curWidget,
					// Child's local position (i.e. position within parent)
					localPosition,
					// Child's size
					localSize
				));
			}
		}
	}
}

std::wstring SCanvasPanel::GenerateAutoNumberedName()
{
    static std::atomic<size_t> number;
    static std::wstring wc_name = ANSI_TO_WCHAR(typeid(SPanel).name());
    return std::format(L"{}_{}", wc_name, number++);
}