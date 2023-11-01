// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Widgets/Layout/SCanvasPanel.h"

SCanvasPanel::SCanvasPanel()
{
}

SCanvasPanel::~SCanvasPanel() noexcept
{
}

SCanvasPanel::NSlot& SCanvasPanel::AddSlot()
{
	NSlot& NewSlot = Slots.emplace_back();
	InvalidateLayoutAndVolatility();
	return NewSlot;
}

bool SCanvasPanel::RemoveSlot(size_t Index)
{
	if (Slots.size() <= Index)
	{
		return false;
	}

	Slots.erase(Slots.begin() + Index);
	InvalidateLayoutAndVolatility();
	return true;
}

size_t SCanvasPanel::FindSlot(const SWidget& Content) const
{
	for (size_t i = 0; i < Slots.size(); ++i)
	{
		if (Slots[i].Content.get() == &Content)
		{
			return i;
		}
	}

	return IntegralTypes::Npos;
}

void SCanvasPanel::ClearSlots()
{
	Slots.clear();
	InvalidateLayoutAndVolatility();
}

Vector2 SCanvasPanel::ComputeDesiredSize() const
{
	Vector2 FinalDesiredSize(0, 0);

	// Arrange the children now in their proper z-order.
	for (size_t i = 0; i < Slots.size(); ++i)
	{
		const NSlot& CurChild = Slots[i];
		const SWidget* Widget = CurChild.Content.get();
		if (Widget == nullptr)
		{
			continue;
		}

		// As long as the widgets are not collapsed, they should contribute to the desired size.
		ESlateVisibility::Enum ChildVisibility = Widget->GetVisibility();
		if (ChildVisibility == ESlateVisibility::Collapsed)
		{
			continue;
		}

		const NMargin& Offset = CurChild.GetOffset();
		const NAnchors& Anchors = CurChild.GetAnchors();

		const Vector2& SlotSize = Vector2(Offset.Right, Offset.Bottom);
		const bool& bAutoSize = CurChild.IsAutoSize();
		const Vector2 Size = bAutoSize ? Widget->GetDesiredSize() : SlotSize;

		const bool bIsDockedHorizontally = (Anchors.Minimum.X == Anchors.Maximum.X) && (Anchors.Minimum.X == 0 || Anchors.Minimum.X == 1);
		const bool bIsDockedVertically = (Anchors.Minimum.Y == Anchors.Maximum.Y) && (Anchors.Minimum.Y == 0 || Anchors.Minimum.Y == 1);

		FinalDesiredSize.X = Math::Max(FinalDesiredSize.X, Size.X + (bIsDockedHorizontally ? Math::Abs(Offset.Left) : 0.0f));
		FinalDesiredSize.Y = Math::Max(FinalDesiredSize.Y, Size.Y + (bIsDockedVertically ? Math::Abs(Offset.Top) : 0.0f));
	}

	return FinalDesiredSize;
}

int32 SCanvasPanel::OnPaint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const
{
	NArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
	std::vector<bool> ChildLayers;
	ArrangeLayeredChildrens(ArrangedChildren, AllottedGeometry, ChildLayers);

	const bool bForwardedEnabled = ShouldBeEnabled(bParentEnabled);

	// Because we paint multiple children, we must track the maximum layer id that they produced in case one of our parents
	// wants to an overlay for all of its contents.
	int32 MaxLayerId = InLayer;
	int32 ChildLayerId = InLayer;
	NPaintArgs NewArgs = Args.WithNewParent(*this);

	const std::vector<NArrangedWidget>& ArrangedWidgets = ArrangedChildren.GetWidgets();
	for (size_t ChildIndex = 0; ChildIndex < ArrangedWidgets.size(); ++ChildIndex)
	{
		const NArrangedWidget& CurWidget = ArrangedWidgets[ChildIndex];

		if (!IsChildWidgetCulled(CullingRect, CurWidget))
		{
			// Bools in ChildLayers tell us whether to paint the next child in front of all previous
			if (ChildLayers[ChildIndex])
			{
				ChildLayerId = MaxLayerId + 1;
			}

			const int32 CurWidgetsMaxLayerId = CurWidget.GetWidget()->Paint(NewArgs, CurWidget.GetGeometry(), CullingRect, OutDrawElements, ChildLayerId, bForwardedEnabled);

			MaxLayerId = Math::Max(MaxLayerId, CurWidgetsMaxLayerId);
		}
	}

	return MaxLayerId;
}

void SCanvasPanel::OnArrangeChildren(NArrangedChildrens& InoutArrangedChildrens, const NGeometry& AllottedGeometry) const
{
	std::vector<bool> ChildLayers;
	ArrangeLayeredChildrens(InoutArrangedChildrens, AllottedGeometry, ChildLayers);
}

void SCanvasPanel::ArrangeLayeredChildrens(NArrangedChildrens& InoutArrangedChildrens, const NGeometry& AllottedGeometry, std::vector<bool>& ChildLayers) const
{
	if (Slots.empty())
	{
		return;
	}

	ChildLayers.reserve(Slots.size());

	// Sort the children based on z-order.
	std::vector<NChildZOrder> SlotOrder;
	SlotOrder.reserve(Slots.size());

	for (size_t i = 0; i < Slots.size(); ++i)
	{
		auto& CurChild = Slots[i];
		SlotOrder.emplace_back() =
		{
			.ChildIndex = i,
			.ZOrder = CurChild._ZOrder
		};
	}

	static auto SortSlotsByZOrder = [](const NChildZOrder& Lhs, const NChildZOrder& Rhs)
	{
		return Lhs.ZOrder == Rhs.ZOrder
			? std::less<size_t>()(Lhs.ChildIndex, Rhs.ChildIndex)
			: std::less<int32>()(Lhs.ZOrder, Rhs.ZOrder);
	};

	std::sort(SlotOrder.begin(), SlotOrder.end(), SortSlotsByZOrder);
	int32 LastZOrder = std::numeric_limits<int32>::lowest();

	for (size_t ChildIndex = 0; ChildIndex < Slots.size(); ++ChildIndex)
	{
		const NChildZOrder& CurSlot = SlotOrder[ChildIndex];
		const NSlot& CurChild = Slots[CurSlot.ChildIndex];
		const std::shared_ptr<SWidget>& CurWidget = CurChild.Content;

		ESlateVisibility::Enum ChildVisibility = CurWidget->GetVisibility();
		if (InoutArrangedChildrens.Accepts(ChildVisibility) == false)
		{
			continue;
		}

		const NMargin& Offset = CurChild.GetOffset();
		const Vector2& Alignment = CurChild.GetAlignment();
		const NAnchors& Anchors = CurChild.GetAnchors();
		const bool bAutoSize = CurChild.IsAutoSize();

		const NMargin AnchorPixels(
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
		if (CurSlot.ZOrder > LastZOrder)
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

DEFINE_SLATE_CONSTRUCTOR(SCanvasPanel, Args)
{
	Slots = std::move(Args.Slots);
}