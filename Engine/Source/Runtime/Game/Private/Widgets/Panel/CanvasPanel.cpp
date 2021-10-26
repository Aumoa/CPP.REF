// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Panel/CanvasPanel.h"
#include "Layout/ArrangedChildrens.h"
#include "Draw/PaintArgs.h"

SCanvasPanel::SCanvasPanel() : Super()
{
}

SCanvasPanel::~SCanvasPanel()
{
}

Vector2 SCanvasPanel::GetDesiredSize()
{
	Vector2 finalDesiredSize(0, 0);

	// Arrange the children now in their proper z-order.
	for (size_t i = 0; i < _slots.size(); ++i)
	{
		Slot& curChild = _slots[i];
		SWidget* widget = curChild.GetContent();
		ESlateVisibility childVisibility = widget->GetVisibility();

		// As long as the widgets are not collapsed, they should contribute to the desired size.
		if (childVisibility != ESlateVisibility::Collapsed)
		{
			const Margin& offset = curChild._Offset;
			const Anchors& anchors = curChild._Anchors;

			const Vector2& slotSize = Vector2(offset.Right, offset.Bottom);

			const bool& bAutoSize = curChild._bAutoSize;

			const Vector2 size = bAutoSize ? widget->GetDesiredSize() : slotSize;

			const bool bIsDockedHorizontally = (anchors.Minimum.X == anchors.Maximum.X) && (anchors.Minimum.X == 0 || anchors.Minimum.X == 1);
			const bool bIsDockedVertically = (anchors.Minimum.Y == anchors.Maximum.Y) && (anchors.Minimum.Y == 0 || anchors.Minimum.Y == 1);

			finalDesiredSize.X = MathEx::Max(finalDesiredSize.X, size.X + (bIsDockedHorizontally ? MathEx::Abs(offset.Left) : 0.0f));
			finalDesiredSize.Y = MathEx::Max(finalDesiredSize.Y, size.Y + (bIsDockedVertically ? MathEx::Abs(offset.Top) : 0.0f));
		}
	}

	return finalDesiredSize;
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

size_t SCanvasPanel::FindSlot(const SWidget* content)
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

size_t SCanvasPanel::NumSlots()
{
	return _slots.size();
}

DEFINE_SLATE_CONSTRUCTOR(SCanvasPanel, attr)
{
	Super::Construct(attr);
	_slots = std::move(attr.Slots);
}

int32 SCanvasPanel::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled)
{
	ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
	std::vector<bool> ChildLayers;
	ArrangeLayeredChildren(ArrangedChildren, AllottedGeometry, ChildLayers);

	const bool bForwardedEnabled = ShouldBeEnabled(bParentEnabled);

	// Because we paint multiple children, we must track the maximum layer id that they produced in case one of our parents
	// wants to an overlay for all of its contents.
	int32 MaxLayerId = InLayer;
	int32 ChildLayerId = InLayer;

	const PaintArgs NewArgs = Args.WithNewParent(this);

	const std::vector<ArrangedWidget>& ArrangedWidgets = ArrangedChildren.GetWidgets();
	for (size_t ChildIndex = 0; ChildIndex < ArrangedWidgets.size(); ++ChildIndex)
	{
		const ArrangedWidget& CurWidget = ArrangedWidgets[ChildIndex];

		if (!IsChildWidgetCulled(CullingRect, CurWidget))
		{
			// Bools in ChildLayers tell us whether to paint the next child in front of all previous
			if (ChildLayers[ChildIndex])
			{
				ChildLayerId = MaxLayerId + 1;
			}

			const int32 CurWidgetsMaxLayerId = CurWidget.GetWidget()->Paint(NewArgs, CurWidget.GetGeometry(), CullingRect, InDrawElements, ChildLayerId, bForwardedEnabled);

			MaxLayerId = MathEx::Max(MaxLayerId, CurWidgetsMaxLayerId);
		}
	}

	return MaxLayerId;
}

void SCanvasPanel::OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry)
{
	std::vector<bool> ChildLayers;
	ArrangeLayeredChildren(InoutArrangedChildrens, AllottedGeometry, ChildLayers);
}

void SCanvasPanel::ArrangeLayeredChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry, std::vector<bool>& ChildLayers)
{
	if (_slots.size() > 0)
	{
		ChildLayers.reserve(_slots.size());

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
		float LastZOrder = std::numeric_limits<float>::min();

		// Arrange the children now in their proper z-order.
		for (size_t childIndex = 0; childIndex < _slots.size(); ++childIndex)
		{
			ChildZOrder curSlot = slotOrder[childIndex];
			const Slot& curChild = _slots[curSlot.ChildIndex];
			SWidget* curWidget = curChild.GetContent();

			ESlateVisibility childVisibility = curWidget->GetVisibility();
			if (InoutArrangedChildrens.Accepts(childVisibility))
			{
				const Margin& Offset = curChild._Offset;
				const Vector2& Alignment = curChild._Alignment;
				const Anchors& Anchors = curChild._Anchors;
				const bool bAutoSize = curChild._bAutoSize;

				const Margin AnchorPixels(
					Anchors.Minimum.X * AllottedGeometry.GetSize().X,
					Anchors.Minimum.Y * AllottedGeometry.GetSize().Y,
					Anchors.Maximum.X * AllottedGeometry.GetSize().X,
					Anchors.Maximum.Y * AllottedGeometry.GetSize().Y
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
				InoutArrangedChildrens.AddWidget(childVisibility, AllottedGeometry.MakeChild(
					// The child widget being arranged
					curWidget,
					// Child's local position (i.e. position within parent)
					localPosition,
					// Child's size
					localSize
				));

				// Split children into discrete layers for the paint method
				bool bNewLayer = false;
				if (curSlot.ZOrder > LastZOrder + MathEx::SmallNumber)
				{
					if (ChildLayers.size() > 0)
					{
						bNewLayer = true;
					}
					LastZOrder = curSlot.ZOrder;
				}
				ChildLayers.emplace_back(bNewLayer);
			}
		}
	}
}