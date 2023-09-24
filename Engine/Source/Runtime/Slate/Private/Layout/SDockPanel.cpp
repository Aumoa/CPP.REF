// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Layout/SDockPanel.h"
#include "Layout/AlignmentArrangeResult.h"

SDockPanel::SDockPanel()
{
}

SDockPanel::~SDockPanel() noexcept
{
}

SDockPanel::NSlot& SDockPanel::AddSlot()
{
	NSlot& NewSlot = Slots.emplace_back();
	InvalidateLayoutAndVolatility();
	return NewSlot;
}

bool SDockPanel::RemoveSlot(size_t Index)
{
	if (Slots.size() <= Index)
	{
		return false;
	}

	Slots.erase(Slots.begin() + Index);
	InvalidateLayoutAndVolatility();
	return true;
}

size_t SDockPanel::FindSlot(const SWidget& Content) const
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

void SDockPanel::ClearSlots()
{
	Slots.clear();
	InvalidateLayoutAndVolatility();
}

Vector2 SDockPanel::ComputeDesiredSize() const
{
	Vector2 VerticalDesiredSize;
	Vector2 HorizontalDesiredSize;

	const NSlot* CenterContent = nullptr;
	for (auto& CurChild : Slots)
	{
		const SWidget* Widget = CurChild.Content.get();
		if (Widget == nullptr)
		{
			continue;
		}

		ESlateVisibility::Enum ChildVisibility = Widget->GetVisibility();
		if (ChildVisibility != ESlateVisibility::Collapsed)
		{
			continue;
		}

		Vector2 DesiredSize = CurChild.Content->GetDesiredSize();
		DesiredSize += CurChild._SlotPadding;

		switch (CurChild._Dock)
		{
		case EDockAttach::Left:
		case EDockAttach::Right:
			VerticalDesiredSize.X += DesiredSize.X;
			VerticalDesiredSize.Y = Math::Max(HorizontalDesiredSize.Y + DesiredSize.Y, VerticalDesiredSize.Y);
			break;
		case EDockAttach::Top:
		case EDockAttach::Bottom:
			HorizontalDesiredSize.X = Math::Max(VerticalDesiredSize.X + DesiredSize.X, HorizontalDesiredSize.X);
			HorizontalDesiredSize.Y += DesiredSize.Y;
			break;
		default:
			check(CenterContent == nullptr);
			CenterContent = &CurChild;
			break;
		}
	}

	Vector2 MaxDesiredSize = Vector2(Math::Max(VerticalDesiredSize.X, HorizontalDesiredSize.X), Math::Max(VerticalDesiredSize.Y, HorizontalDesiredSize.Y));
	if (CenterContent)
	{
		Vector2 DesiredSize = CenterContent->Content->GetDesiredSize();
		MaxDesiredSize += DesiredSize;
	}

	return MaxDesiredSize;
}

int32 SDockPanel::OnPaint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const
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

void SDockPanel::OnArrangeChildren(NArrangedChildrens& InoutArrangedChildrens, const NGeometry& AllottedGeometry) const
{
	if (Slots.empty())
	{
		return;
	}

	const NSlot* CenterContent = nullptr;
	Rect LocalEntireRect = Rect(Vector2::Zero(), AllottedGeometry.GetLocalSize());

	for (auto& ChildSlot : Slots)
	{
		const std::shared_ptr<SWidget>& CurWidget = ChildSlot.Content;

		ESlateVisibility::Enum ChildVisibility = CurWidget->GetVisibility();
		if (InoutArrangedChildrens.Accepts(ChildVisibility) == false)
		{
			continue;
		}

		if (ChildSlot._Dock == EDockAttach::Center)
		{
			check(CenterContent == nullptr);
			CenterContent = &ChildSlot;
			continue;
		}

		const EFlowDirection MyFlowDirection = GetFlowDirection();
		const NMargin SlotPadding = LayoutPaddingWithFlow(ChildSlot._SlotPadding, MyFlowDirection);

		Vector2 LocalSize = Vector2(Rect::Width(LocalEntireRect), Rect::Height(LocalEntireRect));
		const Vector2 DesiredSize = CurWidget->GetDesiredSize();

		switch (ChildSlot._Dock)
		{
		case EDockAttach::Left:
		case EDockAttach::Right:
			LocalSize.X = Math::Min(DesiredSize.X + SlotPadding.GetTotalSpaceAlong(EOrientation::Horizontal), LocalSize.X);
			break;
		case EDockAttach::Top:
		case EDockAttach::Bottom:
			LocalSize.Y = Math::Min(DesiredSize.Y + SlotPadding.GetTotalSpaceAlong(EOrientation::Vertical), LocalSize.Y);
			break;
		}

		const auto XResult = NAlignmentArrangeResult::AlignChild(EOrientation::Horizontal, MyFlowDirection, Rect::Width(LocalEntireRect), ChildSlot, SlotPadding);
		const auto YResult = NAlignmentArrangeResult::AlignChild(EOrientation::Vertical, MyFlowDirection, Rect::Height(LocalEntireRect), ChildSlot, SlotPadding);

		Vector2 LocalPosition = Rect::LeftTop(LocalEntireRect);
		switch (ChildSlot._Dock)
		{
		case EDockAttach::Left:
			LocalEntireRect.Left += XResult.Size + SlotPadding.GetTotalSpaceAlong(EOrientation::Horizontal);
			break;
		case EDockAttach::Top:
			LocalEntireRect.Top += YResult.Size + SlotPadding.GetTotalSpaceAlong(EOrientation::Vertical);
			break;
		case EDockAttach::Right:
			LocalPosition.X = LocalEntireRect.Right - (XResult.Size + SlotPadding.GetTotalSpaceAlong(EOrientation::Horizontal));
			LocalEntireRect.Right -= XResult.Size + SlotPadding.GetTotalSpaceAlong(EOrientation::Horizontal);
			break;
		case EDockAttach::Bottom:
			LocalPosition.Y = LocalEntireRect.Bottom - (YResult.Size + SlotPadding.GetTotalSpaceAlong(EOrientation::Vertical));
			LocalEntireRect.Bottom -= YResult.Size + SlotPadding.GetTotalSpaceAlong(EOrientation::Vertical);
			break;
		}

		InoutArrangedChildrens.AddWidget(ChildVisibility, AllottedGeometry.MakeChild(
			CurWidget,
			LocalPosition + Vector2(XResult.Offset, YResult.Offset),
			Vector2(XResult.Size, YResult.Size)
		));
	}

	if (CenterContent)
	{
		const std::shared_ptr<SWidget>& CurWidget = CenterContent->Content;

		const Vector2 LocalSize = Vector2(Rect::Width(LocalEntireRect), Rect::Height(LocalEntireRect));
		const Vector2 DesiredSize = CenterContent->Content->GetDesiredSize();

		const EFlowDirection MyFlowDirection = GetFlowDirection();
		const NMargin SlotPadding = LayoutPaddingWithFlow(CenterContent->_SlotPadding, MyFlowDirection);
		const auto XResult = NAlignmentArrangeResult::AlignChild(EOrientation::Horizontal, MyFlowDirection, Rect::Width(LocalEntireRect), *CenterContent, SlotPadding);
		const auto YResult = NAlignmentArrangeResult::AlignChild(EOrientation::Vertical, MyFlowDirection, Rect::Height(LocalEntireRect), *CenterContent, SlotPadding);

		Vector2 LocalPosition = Rect::LeftTop(LocalEntireRect);

		InoutArrangedChildrens.AddWidget(CurWidget->GetVisibility(), AllottedGeometry.MakeChild(
			CurWidget,
			LocalPosition + Vector2(XResult.Offset, YResult.Offset),
			Vector2(XResult.Size, YResult.Size)
		));
	}
}

DEFINE_SLATE_CONSTRUCTOR(SDockPanel, Args)
{
	Slots = std::move(Args.Slots);
}