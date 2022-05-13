// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/Viewports/SWindow.h"
#include "Widgets/Viewports/SViewport.h"

using namespace ::libty;

SWindow::SWindow()
	: Super()
{
}

void SWindow::Inject(SRenderEngine* REngine)
{
	RContext = REngine->CreateRenderContext();
}

void SWindow::RenderWindow(SRenderContext* RContext)
{
	SwapChain->Present();
}

size_t SWindow::NumChildrens()
{
	return Viewports.size();
}

SWidget* SWindow::GetChildrenAt(size_t IndexOf)
{
	return Viewports[IndexOf];
}

void SWindow::OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry)
{
	for (auto& Vp : Viewports)
	{
		InoutArrangedChildrens.AddWidget(Vp->GetVisibility(), AllottedGeometry.MakeChild(
			Vp,
			Vector2::Zero(),
			AllottedGeometry.GetLocalSize()
		));
	}
}

DEFINE_SLATE_CONSTRUCTOR(SWindow, Attr)
{
	BufferCount = Attr._BufferCount;

	IRHIFactory* Factory = RContext->OwningDevice->GetFactory();
	SwapChain = Factory->CreateSwapChain(RContext->RenderQueue, BufferCount);

	Viewports.emplace_back(SNew(SViewport));

	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);
}