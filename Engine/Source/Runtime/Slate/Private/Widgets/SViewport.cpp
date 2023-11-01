// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/SViewport.h"
#include "Layout/AlignmentArrangeResult.h"
#include "RHI/RHIViewport.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "Rendering/Texture2DTaskRenderAssetProxy.h"
#include "RenderGlobal.h"
#include "Renderer/RayTracingSceneRenderer.h"

SViewport::SViewport()
{
}

SViewport::~SViewport() noexcept
{
}

void SViewport::Tick(const NGeometry& AllottedGeometry, const TimeSpan& InDeltaTime)
{
	PLATFORM_UNREFERENCED_PARAMETER(InDeltaTime);

	if (bAllocateViewport)
	{
		Vector2N ScreenResolution = Vector<>::Cast<int32>(AllottedGeometry.GetLocalSize());
		if (Viewport->GetViewportSize() != ScreenResolution)
		{
			ResizeViewport(ScreenResolution);
		}
	}
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
	bAllocateViewport = Args._bAllocateViewport;
	Slots = std::move(Args.Slots);

	if (bAllocateViewport)
	{
		Viewport = NRHIGlobal::GetDynamicRHI().CreateViewport();
		SceneRenderer = std::make_shared<NRayTracingSceneRenderer>();
	}
	else
	{
		RenderProxy = NRenderGlobal::GetNullRenderProxy();
	}
}

Vector2 SViewport::ComputeDesiredSize() const
{
	Vector2 MaxDesiredSize;

	for (auto& Slot : Slots)
	{
		Vector2 CurrentDesiredSize = Slot.Content->GetDesiredSize();
		MaxDesiredSize.X = Math::Max(CurrentDesiredSize.X, MaxDesiredSize.X);
		MaxDesiredSize.Y = Math::Max(CurrentDesiredSize.Y, MaxDesiredSize.Y);
	}

	return MaxDesiredSize;
}

int32 SViewport::OnPaint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const
{
	NArrangedChildrens ArrangedChildrens(ESlateVisibility::Visible);
	OnArrangeChildren(ArrangedChildrens, AllottedGeometry);

	const bool bForwardedEnabled = ShouldBeEnabled(bParentEnabled);

	// Viewport elements always render with new layer.
	int32 MaxLayerId = InLayer + 1;
	NPaintArgs NewArgs = Args.WithNewParent(*this);

	NSlateRenderElement& Element = OutDrawElements.Add();
	Element.AbsolutePosition = AllottedGeometry.GetAbsolutePosition();
	Element.Layout = AllottedGeometry.ToPaintGeometry();
	Element.Layer = InLayer;
	Element.RenderOpacity = Args.RenderOpacity;
	Element.TintColor = NamedColors::White;
	Element.Proxy = RenderProxy;

	if (SceneRenderer)
	{
		SceneRenderer->BeginRender();
		SceneRenderer->SetViewport(Viewport);
		SceneRenderer->EndRender();
	}

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

		const EFlowDirection MyFlowDirection = GetFlowDirection();
		const NMargin SlotPadding = LayoutPaddingWithFlow(CurChild._SlotPadding, MyFlowDirection);
		const auto XResult = NAlignmentArrangeResult::AlignChild(EOrientation::Horizontal, MyFlowDirection, AllottedGeometry.GetLocalSize().X, CurChild, SlotPadding);
		const auto YResult = NAlignmentArrangeResult::AlignChild(EOrientation::Vertical, MyFlowDirection, AllottedGeometry.GetLocalSize().Y, CurChild, SlotPadding);

		ArrangedChildrens.AddWidget(ChildVisibility, AllottedGeometry.MakeChild(
			CurWidget,
			Vector2(XResult.Offset, YResult.Offset),
			Vector2(XResult.Size, YResult.Size)
		));
	}
}

void SViewport::ResizeViewport(const Vector2N& NewSize)
{
	if (NewSize.X < 0 || NewSize.Y < 0)
	{
		return;
	}

	Viewport->Resize(NewSize);
	RenderProxy = std::make_shared<NTexture2DTaskRenderAssetProxy>(Task<>::FromResult(Viewport->GetTexture()));
}