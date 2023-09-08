// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Layout/SCanvasPanel.h"

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
	if (Slots.size() >= Index)
	{
		return false;
	}

	Slots.erase(Slots.begin() + Index);
	InvalidateLayoutAndVolatility();
	return true;
}

size_t SCanvasPanel::FindSlot(const SWidget& Content)
{
	for (size_t i = 0; i < Slots.size(); ++i)
	{
		if (Slots[i]._Content.get() == &Content)
		{
			return i;
		}
	}

	return -1;
}

void SCanvasPanel::ClearSlots()
{
	Slots.clear();
	InvalidateLayoutAndVolatility();
}

DEFINE_SLATE_CONSTRUCTOR(SCanvasPanel, Args)
{
	Slots.reserve(Args.Slots.size());
	for (auto& Slot : Args.Slots)
	{
		Slots.emplace_back() = Slot;
	}
}

Vector2 SCanvasPanel::ComputeDesiredSize() const
{
	Vector2 FinalDesiredSize(0, 0);

	// Arrange the children now in their proper z-order.
	for (size_t i = 0; i < Slots.size(); ++i)
	{
		const NSlot& CurChild = Slots[i];
		const SWidget* Widget = CurChild.GetContent();
		if (Widget == nullptr)
		{
			continue;
		}

		ESlateVisibility::Enum ChildVisibility = Widget->GetVisibility();

		// As long as the widgets are not collapsed, they should contribute to the desired size.
		if (ChildVisibility != ESlateVisibility::Collapsed)
		{
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