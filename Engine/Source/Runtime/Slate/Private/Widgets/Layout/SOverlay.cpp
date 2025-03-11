// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Widgets/Layout/SOverlay.h"
#include "Layout/AlignmentArrangeResult.h"

SOverlay::SOverlay()
{
}

SOverlay::~SOverlay() noexcept
{
}

SOverlay::NSlot& SOverlay::AddSlot()
{
	NSlot& NewSlot = Slots.emplace_back();
	InvalidateLayoutAndVolatility();
	return NewSlot;
}

bool SOverlay::RemoveSlot(size_t Index)
{
	if (Slots.size() <= Index)
	{
		return false;
	}

	Slots.erase(Slots.begin() + Index);
	InvalidateLayoutAndVolatility();
	return true;
}

size_t SOverlay::FindSlot(const SWidget& Content) const
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

void SOverlay::ClearSlots()
{
	Slots.clear();
	InvalidateLayoutAndVolatility();
}

Vector2 SOverlay::ComputeDesiredSize() const
{
	Vector2 MaxDesiredSize;
	for (auto& Slot : Slots)
	{
		Vector2 DesiredSize = Slot.Content->GetDesiredSize();
		MaxDesiredSize = Vector<>::Max(MaxDesiredSize, DesiredSize);
	}
	return MaxDesiredSize;
}

int32 SOverlay::OnPaint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const
{
	NArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
	std::vector<bool> ChildLayers;
	OnArrangeChildren(ArrangedChildren, AllottedGeometry);

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
			ChildLayerId = MaxLayerId + 1;

			const int32 CurWidgetsMaxLayerId = CurWidget.GetWidget()->Paint(NewArgs, CurWidget.GetGeometry(), CullingRect, OutDrawElements, ChildLayerId, bForwardedEnabled);

			MaxLayerId = Math::Max(MaxLayerId, CurWidgetsMaxLayerId);
		}
	}

	return MaxLayerId;
}

void SOverlay::OnArrangeChildren(NArrangedChildrens& InoutArrangedChildrens, const NGeometry& AllottedGeometry) const
{
	if (Slots.empty())
	{
		return;
	}

	for (size_t ChildIndex = 0; ChildIndex < Slots.size(); ++ChildIndex)
	{
		const NSlot& ChildSlot = Slots[ChildIndex];
		const std::shared_ptr<SWidget>& CurWidget = ChildSlot.Content;

		ESlateVisibility::Enum ChildVisibility = CurWidget->GetVisibility();
		if (InoutArrangedChildrens.Accepts(ChildVisibility) == false)
		{
			continue;
		}

		const EFlowDirection MyFlowDirection = GetFlowDirection();
		const NMargin SlotPadding = LayoutPaddingWithFlow(ChildSlot._SlotPadding, MyFlowDirection);
		const auto XResult = NAlignmentArrangeResult::AlignChild(EOrientation::Horizontal, MyFlowDirection, AllottedGeometry.GetLocalSize().X, ChildSlot, SlotPadding);
		const auto YResult = NAlignmentArrangeResult::AlignChild(EOrientation::Vertical, MyFlowDirection, AllottedGeometry.GetLocalSize().Y, ChildSlot, SlotPadding);

		InoutArrangedChildrens.AddWidget(ChildVisibility, AllottedGeometry.MakeChild(
			CurWidget,
			Vector2(XResult.Offset, YResult.Offset),
			Vector2(XResult.Size, YResult.Size)
		));
	}
}

DEFINE_SLATE_CONSTRUCTOR(SOverlay, Args)
{
	Slots = std::move(Args.Slots);
}