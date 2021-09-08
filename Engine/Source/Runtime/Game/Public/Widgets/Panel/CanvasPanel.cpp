// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Widgets/Panel/CanvasPanel.h"
#include "Layout/ArrangedChildrens.h"
#include "Draw/PaintArgs.h"

SCanvasPanel::SCanvasPanel(const std::wstring& name) : Super(name)
{
}

SCanvasPanel::~SCanvasPanel()
{
}

auto SCanvasPanel::AddSlot() -> Slot&
{
	return _slots.emplace_back();
}

bool SCanvasPanel::RemoveSlot(size_t index)
{
	if (_slots.size() <= index)
	{
		return false;
	}

	_slots.erase(_slots.begin() + index);
	return true;
}

size_t SCanvasPanel::FindSlot(const SWidget* content) const
{
	for (size_t i = 0; i < _slots.size(); ++i)
	{
		if (_slots[i].GetContent() == content)
		{
			return i;
		}
	}

	return -1;
}

void SCanvasPanel::ClearSlots()
{
	_slots.resize(0);
}

size_t SCanvasPanel::NumSlots() const
{
	return _slots.size();
}

DEFINE_SLATE_CONSTRUCTOR(SCanvasPanel, attr)
{
	Super::Construct(attr);
	_slots = std::move(attr.Slots);
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
	if (_slots.size() > 0)
	{
		// Sort the children based on zorder.
		std::vector<ChildZOrder> slotOrder;
		slotOrder.reserve(_slots.size());

		for (size_t i = 0; i < _slots.size(); ++i)
		{
			auto& curChild = _slots[i];
			slotOrder.emplace_back() =
			{
				.ChildIndex = (int32)i,
				.ZOrder = curChild._ZOrder
			};
		}

		static auto compareAscending = [](auto lh, auto rh)
		{
			return lh < rh;
		};

		static auto sortSlotsByZOrder = [](auto& lhs, auto& rhs)
		{
			return lhs.ZOrder == rhs.ZOrder
				? compareAscending(lhs.ChildIndex, rhs.ChildIndex)
				: compareAscending(lhs.ZOrder, rhs.ZOrder);
		};

		std::sort(slotOrder.begin(), slotOrder.end(), sortSlotsByZOrder);

		// Arrange the children now in their proper z-order.
		for (size_t childIndex = 0; childIndex < _slots.size(); ++childIndex)
		{
			ChildZOrder curSlot = slotOrder[childIndex];
			const Slot& curChild = _slots[curSlot.ChildIndex];
			const SWidget* curWidget = curChild.GetContent();

			ESlateVisibility childVisibility = curWidget->GetVisibility();
			if (arrangedChildrens->Accepts(childVisibility))
			{
				const Margin& Offset = curChild._Offset;
				const Vector2& Alignment = curChild._Alignment;
				const Anchors& Anchors = curChild._Anchors;
				const bool bAutoSize = curChild._bAutoSize;

				const Margin AnchorPixels(
					Anchors.Minimum.X * allottedGeometry.GetSize().X,
					Anchors.Minimum.Y * allottedGeometry.GetSize().Y,
					Anchors.Maximum.X * allottedGeometry.GetSize().X,
					Anchors.Maximum.Y * allottedGeometry.GetSize().Y
				);

				const bool bIsHorizontalStretch = Anchors.Minimum.X != Anchors.Maximum.X;
				const bool bIsVerticalStretch = Anchors.Minimum.Y != Anchors.Maximum.Y;

				const auto SlotSize = Vector2(Offset.Right, Offset.Bottom);
				const auto Size = bAutoSize ? curWidget->GetDesiredSize() : SlotSize;

				// Calculate the offset based on the pivot position.
				const Vector2 AlignmentOffset = Size * Alignment;

				// Calculate the local position based on the anchor and position offset.
				Vector2 localPosition, localSize;

				// Calculate the position and size based on the horizontal stretch or non-stretch
				if (bIsHorizontalStretch)
				{
					localPosition.X = AnchorPixels.Left + Offset.Left;
					localSize.X = AnchorPixels.Right - localPosition.X - Offset.Right;
				}
				else
				{
					localPosition.X = AnchorPixels.Left + Offset.Left - AlignmentOffset.X;
					localSize.X = Size.X;
				}

				// Calculate the position and size based on the vertical stretch or non-stretch
				if (bIsVerticalStretch)
				{
					localPosition.Y = AnchorPixels.Top + Offset.Top;
					localSize.Y = AnchorPixels.Bottom - localPosition.Y - Offset.Bottom;
				}
				else
				{
					localPosition.Y = AnchorPixels.Top + Offset.Top - AlignmentOffset.Y;
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