// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/SCompoundWidget.h"
#include "Layout/AlignmentArrangeResult.h"

SCompoundWidget::SCompoundWidget()
{
}

void SCompoundWidget::PrepassLayout()
{
	if (ChildSlot.Content && ChildSlot.Content->GetVisibility() != ESlateVisibility::Collapsed)
	{
		ChildSlot.Content->PrepassLayout();
	}

	CacheDesiredSize();
}

void SCompoundWidget::SetContent(std::shared_ptr<SWidget> InContent)
{
	if (ChildSlot.Content != InContent)
	{
		ChildSlot.Content = InContent;
		InvalidateLayoutAndVolatility();
	}
}

Vector2 SCompoundWidget::ComputeDesiredSize() const
{
	ESlateVisibility::Enum ChildVisibility = ChildSlot.Content->GetVisibility();
	if (ChildVisibility != ESlateVisibility::Collapsed)
	{
		return ChildSlot.Content->GetDesiredSize() + ChildSlot._SlotPadding.GetTotalSpaceAlong();
	}

	return Vector2::Zero();
}

void SCompoundWidget::OnArrangeChildren(NArrangedChildrens& ArrangedChildrens, const NGeometry& AllottedGeometry) const
{
	const ESlateVisibility::Enum ChildVisibility = ChildSlot.Content->GetVisibility();
	if (ArrangedChildrens.Accepts(ChildVisibility))
	{
		const EFlowDirection MyFlowDirection = GetFlowDirection();
		const NMargin SlotPadding = LayoutPaddingWithFlow(ChildSlot._SlotPadding, MyFlowDirection);
		const auto XResult = NAlignmentArrangeResult::AlignChild(EOrientation::Horizontal, MyFlowDirection, AllottedGeometry.GetLocalSize().X, ChildSlot, SlotPadding);
		const auto YResult = NAlignmentArrangeResult::AlignChild(EOrientation::Vertical, MyFlowDirection, AllottedGeometry.GetLocalSize().Y, ChildSlot, SlotPadding);

		ArrangedChildrens.AddWidget(ChildVisibility, AllottedGeometry.MakeChild(
			ChildSlot.Content,
			Vector2(XResult.Offset, YResult.Offset),
			Vector2(XResult.Size, YResult.Size)
		));
	}
}

int32 SCompoundWidget::OnPaint([[maybe_unused]] const NPaintArgs& Args, [[maybe_unused]] const NGeometry& AllottedGeometry, [[maybe_unused]] const Rect& CullingRect, [[maybe_unused]] NSlateWindowElementList& OutDrawElements, int32 InLayer, [[maybe_unused]] bool bParentEnabled) const
{
	NArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
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
			const int32 CurWidgetsMaxLayerId = CurWidget.GetWidget()->Paint(NewArgs, CurWidget.GetGeometry(), CullingRect, OutDrawElements, ChildLayerId, bForwardedEnabled);
			MaxLayerId = Math::Max(MaxLayerId, CurWidgetsMaxLayerId);
		}
	}

	return MaxLayerId;
}