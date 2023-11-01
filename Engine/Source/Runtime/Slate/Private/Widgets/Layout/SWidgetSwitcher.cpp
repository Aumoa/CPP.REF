// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Layout/AlignmentArrangeResult.h"

SWidgetSwitcher::SWidgetSwitcher()
{
}

SWidgetSwitcher::~SWidgetSwitcher() noexcept
{
}

SWidgetSwitcher::NSlot& SWidgetSwitcher::AddSlot()
{
	NSlot& NewSlot = Slots.emplace_back();
	InvalidateLayoutAndVolatility();
	return NewSlot;
}

bool SWidgetSwitcher::RemoveSlot(size_t Index)
{
	if (Slots.size() <= Index)
	{
		return false;
	}

	Slots.erase(Slots.begin() + Index);
	InvalidateLayoutAndVolatility();
	return true;
}

size_t SWidgetSwitcher::FindSlot(const SWidget& Content) const
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

void SWidgetSwitcher::ClearSlots()
{
	Slots.clear();
	InvalidateLayoutAndVolatility();
}

void SWidgetSwitcher::SetActiveWidgetIndex(int32 InIndex)
{
	SlotIndex = InIndex;
	InvalidateLayoutAndVolatility();
}

Vector2 SWidgetSwitcher::ComputeDesiredSize() const
{
	if (Slots.empty())
	{
		return Vector2::Zero();
	}

	// Any children are not visible.
	if ((Slots | Linq::IsValidIndex(SlotIndex)) == false)
	{
		return Vector2::Zero();
	}

	const NSlot& ChildSlot = Slots[SlotIndex];
	return ChildSlot.Content->GetDesiredSize();
}

int32 SWidgetSwitcher::OnPaint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const
{
	NArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
	OnArrangeChildren(ArrangedChildren, AllottedGeometry);

	const bool bForwardedEnabled = ShouldBeEnabled(bParentEnabled);
	NPaintArgs NewArgs = Args.WithNewParent(*this);

	const std::vector<NArrangedWidget>& ArrangedWidgets = ArrangedChildren.GetWidgets();
	checkf(ArrangedWidgets.size() <= 1, TEXT("WidgetSwitcher can active one widget only."));
	if (ArrangedWidgets.empty())
	{
		return InLayer;
	}

	const NArrangedWidget& CurWidget = ArrangedWidgets[0];
	if (IsChildWidgetCulled(CullingRect, CurWidget))
	{
		return InLayer;
	}

	int32 ChildLayerId = CurWidget.GetWidget()->Paint(NewArgs, CurWidget.GetGeometry(), CullingRect, OutDrawElements, InLayer, bForwardedEnabled);
	return Math::Max(ChildLayerId, InLayer);
}

void SWidgetSwitcher::OnArrangeChildren(NArrangedChildrens& InoutArrangedChildrens, const NGeometry& AllottedGeometry) const
{
	if (Slots.empty())
	{
		return;
	}

	// Any children are not visible.
	if ((Slots | Linq::IsValidIndex(SlotIndex)) == false)
	{
		return;
	}

	const NSlot& ChildSlot = Slots[SlotIndex];
	const std::shared_ptr<SWidget>& CurWidget = ChildSlot.Content;

	ESlateVisibility::Enum ChildVisibility = CurWidget->GetVisibility();
	if (InoutArrangedChildrens.Accepts(ChildVisibility) == false)
	{
		return;
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

DEFINE_SLATE_CONSTRUCTOR(SWidgetSwitcher, Args)
{
	Slots = std::move(Args.Slots);
	SlotIndex = Args._SlotIndex;
}