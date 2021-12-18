// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Panel/CanvasPanel.h"
#include "Layout/ArrangedChildrens.h"
#include "Draw/PaintArgs.h"

GENERATE_BODY(SCanvasPanel);

SCanvasPanel::SCanvasPanel() : Super()
{
}

SCanvasPanel::~SCanvasPanel()
{
}

Vector2 SCanvasPanel::GetDesiredSize()
{
	Vector2 FinalDesiredSize(0, 0);

	// Arrange the children now in their proper z-order.
	for (size_t i = 0; i < Slots.size(); ++i)
	{
		Slot& CurChild = Slots[i];
		SWidget* Widget = CurChild.GetContent();
		ESlateVisibility ChildVisibility = Widget->GetVisibility();

		// As long as the widgets are not collapsed, they should contribute to the desired size.
		if (ChildVisibility != ESlateVisibility::Collapsed)
		{
			const Margin& Offset = CurChild._Offset;
			const Anchors& Anchors = CurChild._Anchors;

			const Vector2& SlotSize = Vector2(Offset.Right, Offset.Bottom);
			const bool& bAutoSize = CurChild._bAutoSize;
			const Vector2 Size = bAutoSize ? Widget->GetDesiredSize() : SlotSize;

			const bool bIsDockedHorizontally = (Anchors.Minimum.X == Anchors.Maximum.X) && (Anchors.Minimum.X == 0 || Anchors.Minimum.X == 1);
			const bool bIsDockedVertically = (Anchors.Minimum.Y == Anchors.Maximum.Y) && (Anchors.Minimum.Y == 0 || Anchors.Minimum.Y == 1);

			FinalDesiredSize.X = MathEx::Max(FinalDesiredSize.X, Size.X + (bIsDockedHorizontally ? MathEx::Abs(Offset.Left) : 0.0f));
			FinalDesiredSize.Y = MathEx::Max(FinalDesiredSize.Y, Size.Y + (bIsDockedVertically ? MathEx::Abs(Offset.Top) : 0.0f));
		}
	}

	return FinalDesiredSize;
}

auto SCanvasPanel::AddSlot() -> Slot&
{
	return Slots.emplace_back();
}

bool SCanvasPanel::RemoveSlot(size_t Index)
{
	if (Slots.size() <= Index)
	{
		return false;
	}

	Slots.erase(Slots.begin() + Index);
	return true;
}

size_t SCanvasPanel::FindSlot(const SWidget* Content)
{
	for (size_t i = 0; i < Slots.size(); ++i)
	{
		if (Slots[i].GetContent() == Content)
		{
			return i;
		}
	}

	return -1;
}

void SCanvasPanel::ClearSlots()
{
	Slots.resize(0);
}

size_t SCanvasPanel::NumSlots()
{
	return Slots.size();
}

DEFINE_SLATE_CONSTRUCTOR(SCanvasPanel, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);
	Slots = std::move(Attr.Slots);
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
	if (Slots.size() > 0)
	{
		ChildLayers.reserve(Slots.size());

		// Sort the children based on zorder.
		std::vector<ChildZOrder> SlotOrder;
		SlotOrder.reserve(Slots.size());

		for (size_t i = 0; i < Slots.size(); ++i)
		{
			auto& CurChild = Slots[i];
			SlotOrder.emplace_back() =
			{
				.ChildIndex = (int32)i,
				.ZOrder = CurChild._ZOrder
			};
		}

		static auto CompareAscending = [](auto lh, auto rh)
		{
			return lh < rh;
		};

		static auto SortSlotsByZOrder = [](auto& lhs, auto& rhs)
		{
			return lhs.ZOrder == rhs.ZOrder
				? CompareAscending(lhs.ChildIndex, rhs.ChildIndex)
				: CompareAscending(lhs.ZOrder, rhs.ZOrder);
		};

		std::sort(SlotOrder.begin(), SlotOrder.end(), SortSlotsByZOrder);
		float LastZOrder = std::numeric_limits<float>::min();

		// Arrange the children now in their proper z-order.
		for (size_t ChildIndex = 0; ChildIndex < Slots.size(); ++ChildIndex)
		{
			ChildZOrder CurSlot = SlotOrder[ChildIndex];
			const Slot& CurChild = Slots[CurSlot.ChildIndex];
			SWidget* CurWidget = CurChild.GetContent();

			ESlateVisibility ChildVisibility = CurWidget->GetVisibility();
			if (InoutArrangedChildrens.Accepts(ChildVisibility))
			{
				const Margin& Offset = CurChild._Offset;
				const Vector2& Alignment = CurChild._Alignment;
				const Anchors& Anchors = CurChild._Anchors;
				const bool bAutoSize = CurChild._bAutoSize;

				const Margin AnchorPixels(
					Anchors.Minimum.X * AllottedGeometry.GetSize().X,
					Anchors.Minimum.Y * AllottedGeometry.GetSize().Y,
					Anchors.Maximum.X * AllottedGeometry.GetSize().X,
					Anchors.Maximum.Y * AllottedGeometry.GetSize().Y
				);

				const bool bIsHorizontalStretch = Anchors.Minimum.X != Anchors.Maximum.X;
				const bool bIsVerticalStretch = Anchors.Minimum.Y != Anchors.Maximum.Y;

				const auto SlotSize = Vector2(Offset.Right, Offset.Bottom);
				const auto Size = bAutoSize ? CurWidget->GetDesiredSize() : SlotSize;

				// Calculate the offset based on the pivot position.
				const Vector2 AlignmentOffset = Size * Alignment;

				// Calculate the local position based on the anchor and position offset.
				Vector2 LocalPosition, LocalSize;

				// Calculate the position and size based on the horizontal stretch or non-stretch
				if (bIsHorizontalStretch)
				{
					LocalPosition.X = AnchorPixels.Left + Offset.Left;
					LocalSize.X = AnchorPixels.Right - LocalPosition.X - Offset.Right;
				}
				else
				{
					LocalPosition.X = AnchorPixels.Left + Offset.Left - AlignmentOffset.X;
					LocalSize.X = Size.X;
				}

				// Calculate the position and size based on the vertical stretch or non-stretch
				if (bIsVerticalStretch)
				{
					LocalPosition.Y = AnchorPixels.Top + Offset.Top;
					LocalSize.Y = AnchorPixels.Bottom - LocalPosition.Y - Offset.Bottom;
				}
				else
				{
					LocalPosition.Y = AnchorPixels.Top + Offset.Top - AlignmentOffset.Y;
					LocalSize.Y = Size.Y;
				}

				// Add the information about this child to the output list (ArrangedChildren)
				InoutArrangedChildrens.AddWidget(ChildVisibility, AllottedGeometry.MakeChild(
					// The child widget being arranged
					CurWidget,
					// Child's local position (i.e. position within parent)
					LocalPosition,
					// Child's size
					LocalSize
				));

				// Split children into discrete layers for the paint method
				bool bNewLayer = false;
				if (CurSlot.ZOrder > LastZOrder + MathEx::SmallNumber)
				{
					if (ChildLayers.size() > 0)
					{
						bNewLayer = true;
					}
					LastZOrder = CurSlot.ZOrder;
				}
				ChildLayers.emplace_back(bNewLayer);
			}
		}
	}
}