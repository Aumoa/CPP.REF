// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/SWindow.h"
#include "Widgets/SViewport.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "RHI/RHISwapChain.h"
#include "Rendering/SlateRenderer.h"
#include "GenericPlatform/GenericWindow.h"

SWindow::SWindow()
{
}

SWindow::~SWindow() noexcept
{
}

void SWindow::AttachWindow(std::shared_ptr<NGenericWindow> InNativeWindow)
{
	check(!NativeWindow);
	check(!SwapChain);

	auto& DynamicRHI = NRHIGlobal::GetDynamicRHI();
	NativeWindow = std::move(InNativeWindow);
	SwapChain = DynamicRHI.CreateSwapChain(NRHIGlobal::GetPrimaryCommandQueue(), *NativeWindow);

	UpdateWindowVisibility();
}

void SWindow::ExecuteTick(const TimeSpan& InDeltaTime)
{
	PrepassLayout();

	Vector2 AllottedSize = ComputeDesiredSize();
	if (SwapChain)
	{
		Vector2N VpSize = Vector<>::Cast<int32>(AllottedSize);
		if (SwapChain->GetViewportSize() != VpSize)
		{
			NRHIGlobal::GetDynamicRHI().SyncFrame();
			SwapChain->Resize(VpSize);
		}
	}

	NGeometry AllottedGeometry = NGeometry::MakeRoot(AllottedSize, NSlateLayoutTransform::Identity());
	Tick(AllottedGeometry, InDeltaTime);
}

void SWindow::Render(const TimeSpan& InDeltaTime, NSlateRenderer& Renderer)
{
	Vector2 AllottedSize = ComputeDesiredSize();
	NPaintArgs Args = NPaintArgs::InitPaintArgs(*this, InDeltaTime);
	Rect CullingRect = { Vector2::Zero(), AllottedSize };
	NSlateWindowElementList DrawElements;
	NGeometry AllottedGeometry = NGeometry::MakeRoot(AllottedSize, NSlateLayoutTransform::Identity());
	OnPaint(Args, AllottedGeometry, CullingRect, DrawElements, 0, IsEnabled());

	if (SwapChain)
	{
		Renderer.BeginRender(*SwapChain);
		Renderer.Populate(DrawElements);

		for (auto& Element : DrawElements.UnorderedElements)
		{
			Renderer.RenderElement(Element);
		}
		Renderer.EndRender(*SwapChain);
	}
}

void SWindow::Present()
{
	if (SwapChain)
	{
		SwapChain->Present();
	}
}

DEFINE_SLATE_CONSTRUCTOR(SWindow, Attr)
{
	Attr.SingleSlot._VAlignment = EVerticalAlignment::Fill;
	Attr.SingleSlot._HAlignment = EHorizontalAlignment::Fill;
}

Vector2 SWindow::ComputeDesiredSize() const
{
	return Vector<>::Cast<Vector2>(NativeWindow->GetSize());
}

void SWindow::OnVisibilityChanged(ESlateVisibility::Enum, ESlateVisibility::Enum)
{
	if (NativeWindow)
	{
		UpdateWindowVisibility();
	}
}

void SWindow::UpdateWindowVisibility()
{
	if (ESlateVisibility::IsVisible(GetVisibility()))
	{
		NativeWindow->Show();
	}
	else
	{
		NativeWindow->Hide();
	}
}