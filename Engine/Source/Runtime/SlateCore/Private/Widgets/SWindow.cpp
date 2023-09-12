// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/SWindow.h"
#include "Widgets/SViewport.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "RHI/RHIViewport.h"
#include "GenericPlatform/GenericWindow.h"

SWindow::SWindow()
{
}

SWindow::~SWindow() noexcept
{
}

void SWindow::Tick(const NGeometry& AllottedGeometry, const TimeSpan& InDeltaTime)
{
	if (SlateViewport)
	{
		SlateViewport->Tick(AllottedGeometry, InDeltaTime);
	}
}

void SWindow::AttachWindow(std::shared_ptr<NGenericWindow> InNativeWindow)
{
	check(!NativeWindow);
	check(!Viewport);

	auto& DynamicRHI = NRHIGlobal::GetDynamicRHI();
	NativeWindow = std::move(InNativeWindow);
	Viewport = DynamicRHI.CreateViewport(NRHIGlobal::GetPrimaryCommandQueue(), *NativeWindow);

	UpdateWindowVisibility();
}

void SWindow::ExecuteTick(const TimeSpan& InDeltaTime)
{
	if (IsInvalidated())
	{
		PrepassLayout();
	}

	Vector2 AllottedSize = ComputeDesiredSize();
	NGeometry AllottedGeometry = NGeometry::MakeRoot(AllottedSize, NSlateLayoutTransform::Identity());
	Tick(AllottedGeometry, InDeltaTime);
	NPaintArgs Args = NPaintArgs::InitPaintArgs(*this, InDeltaTime);
	Rect CullingRect = { Vector2::Zero(), AllottedSize };
	NSlateWindowElementList DrawElements;
	OnPaint(Args, AllottedGeometry, CullingRect, DrawElements, 0, IsEnabled());
}

void SWindow::Present()
{
	if (Viewport)
	{
		Viewport->Present();
	}
}

DEFINE_SLATE_CONSTRUCTOR(SWindow, Attr)
{
	PLATFORM_UNREFERENCED_PARAMETER(Attr);
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