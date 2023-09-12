// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/SViewport.h"

SViewport::SViewport()
{
}

SViewport::~SViewport() noexcept
{
}

auto SViewport::AddSlot() -> NSlot&
{
	NSlot& NewSlot = Slots.emplace_back();
	InvalidateLayoutAndVolatility();
	return NewSlot;
}

bool SViewport::RemoveSlot(size_t Index)
{
	if (Slots.size() <= Index)
	{
		return false;
	}

	Slots.erase(Slots.begin() + Index);
	InvalidateLayoutAndVolatility();
	return true;
}

size_t SViewport::FindSlot(const SWidget& Content) const
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

void SViewport::ClearSlots()
{
	Slots.clear();
	InvalidateLayoutAndVolatility();
}

DEFINE_SLATE_CONSTRUCTOR(SViewport, Args)
{
	PLATFORM_UNREFERENCED_PARAMETER(Args);
	Slots = std::move(Args.Slots);
}

Vector2 SViewport::ComputeDesiredSize() const
{
	return Vector2::Zero();
}

int32 SViewport::OnPaint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const
{
	NArrangedChildrens ArrangedChildrens(ESlateVisibility::Visible);
	OnArrangeChildren(ArrangedChildrens, AllottedGeometry);

	const bool bForwardedEnabled = ShouldBeEnabled(bParentEnabled);

	// Viewport elements always render with new layer.
	int32 MaxLayerId = InLayer;
	NPaintArgs NewArgs = Args.WithNewParent(*this);

	const std::vector<NArrangedWidget>& ArrangedWidgets = ArrangedChildrens.GetWidgets();
	for (size_t ChildIndex = 0; ChildIndex < ArrangedWidgets.size(); ++ChildIndex)
	{
		const NArrangedWidget& CurWidget = ArrangedWidgets[ChildIndex];

		if (!IsChildWidgetCulled(CullingRect, CurWidget))
		{
			const int32 CurLayerId = CurWidget.GetWidget()->Paint(NewArgs, CurWidget.GetGeometry(), CullingRect, OutDrawElements, MaxLayerId++, bForwardedEnabled);
			MaxLayerId = Math::Max(MaxLayerId, CurLayerId);
		}
	}

	return MaxLayerId;
}

void SViewport::OnArrangeChildren(NArrangedChildrens& ArrangedChildrens, const NGeometry& AllottedGeometry) const
{
	if (Slots.empty())
	{
		return;
	}

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
	Vector2 AllottedSize = AllottedGeometry.GetLocalSize();

	for (size_t ChildIndex = 0; ChildIndex < Slots.size(); ++ChildIndex)
	{
		const NChildZOrder& CurSlot = SlotOrder[ChildIndex];
		const NSlot& CurChild = Slots[CurSlot.ChildIndex];
		const std::shared_ptr<SWidget>& CurWidget = CurChild.Content;

		ESlateVisibility::Enum ChildVisibility = CurWidget->GetVisibility();
		if (ArrangedChildrens.Accepts(ChildVisibility) == false)
		{
			continue;
		}

		ArrangedChildrens.AddWidget(ChildVisibility, AllottedGeometry.MakeChild(
			CurWidget,
			Vector2::Zero(),
			AllottedSize
		));
	}
}