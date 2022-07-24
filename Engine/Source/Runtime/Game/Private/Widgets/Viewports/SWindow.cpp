// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/Viewports/SWindow.h"
#include "Widgets/Viewports/SViewport.h"
#include "RHI/RHISwapChain.h"
#include "RHI/RHIDevice.h"
#include "EngineCore/GameRenderSubsystem.h"
#include "PlatformMisc/IPlatformWindow.h"

SWindow::SWindow()
	: Super()
{
}

void SWindow::Tick(const Geometry& AllottedGeometry, float InDeltaTime)
{
	Super::Tick(AllottedGeometry, InDeltaTime);
	TryResizeSwapChain(AllottedGeometry);
}

void SWindow::PresentWindow()
{
	_swapChain->Present();
}

size_t SWindow::NumChildrens()
{
	return _viewports.size();
}

SWidget* SWindow::GetChildrenAt(size_t IndexOf)
{
	//return Viewports[IndexOf];
	return nullptr;
}

void SWindow::OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry)
{
	//for (auto& Vp : Viewports)
	//{
	//	InoutArrangedChildrens.AddWidget(Vp->GetVisibility(), AllottedGeometry.MakeChild(
	//		Vp,
	//		Vector2::Zero(),
	//		AllottedGeometry.GetLocalSize()
	//	));
	//}
}

DEFINE_SLATE_CONSTRUCTOR(SWindow, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);

	_device = Attr._RenderSystem->GetDevice();
	_commandQueue = Attr._RenderSystem->GetCommandQueue();
	_platformWindow = Attr._TargetWindow;

	_swapChain = _device->CreateSwapChain(_commandQueue, _platformWindow.Get());
}

void SWindow::TryResizeSwapChain(const Geometry& AllottedGeometry)
{
	Vector2N Size = Vector<>::Cast<int32>(AllottedGeometry.GetLocalSize());
}