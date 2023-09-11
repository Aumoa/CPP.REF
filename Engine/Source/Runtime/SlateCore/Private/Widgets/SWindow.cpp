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

void SWindow::AttachWindow(std::shared_ptr<NGenericWindow> InNativeWindow)
{
	check(!NativeWindow);
	check(!Viewport);

	auto& DynamicRHI = NRHIGlobal::GetDynamicRHI();
	NativeWindow = std::move(InNativeWindow);
	Viewport = DynamicRHI.CreateViewport(NRHIGlobal::GetPrimaryCommandQueue(), *NativeWindow);

	UpdateWindowVisibility();
}

void SWindow::Present()
{
	if (Viewport)
	{
		Viewport->Present();
	}
}

void SWindow::AddViewport(std::shared_ptr<SViewport> InViewport)
{
	SlateViewports.emplace_back(std::move(InViewport));
}

DEFINE_SLATE_CONSTRUCTOR(SWindow, Attr)
{
	PLATFORM_UNREFERENCED_PARAMETER(Attr);
}

Vector2 SWindow::ComputeDesiredSize() const
{
	return Vector<>::Cast<Vector2>(NativeWindow->GetSize());
}

int32 SWindow::OnPaint([[maybe_unused]] const NPaintArgs& Args, [[maybe_unused]] const NGeometry& AllottedGeometry, [[maybe_unused]] const Rect& CullingRect, [[maybe_unused]] NSlateWindowElementList& OutDrawElements, int32 InLayer, [[maybe_unused]] bool bParentEnabled) const
{
	return InLayer;
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