// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/Viewports/SWindow.h"
#include "Widgets/Viewports/SViewport.h"
#include "RHI/RHISwapChain.h"
#include "RHI/RHIDevice.h"
#include "RHI/RHIRenderThread.h"
#include "EngineCore/GameRenderSubsystem.h"
#include "PlatformMisc/IPlatformWindow.h"

constexpr LogCategory LogSlateWindow = TEXT("LogSlateWindow");

SWindow::SWindow()
	: Super()
{
}

SWindow::~SWindow() noexcept
{
}

void SWindow::Tick(const Geometry& AllottedGeometry, const TimeSpan& deltaTime)
{
	Super::Tick(AllottedGeometry, deltaTime);
	TryResizeSwapChain(AllottedGeometry);
}

void SWindow::DispatchSlateTick(const TimeSpan& deltaTime)
{
	Vector2N drawingSize = _platformWindow->GetDrawingSize();
	auto allottedGeometry = Geometry::MakeRoot(Vector<>::Cast<Vector2>(drawingSize), SlateLayoutTransform::Identity());

	Tick(allottedGeometry, deltaTime);
}

void SWindow::PresentWindow()
{
	_swapChain->Present();
}

size_t SWindow::NumChildrens()
{
	return 1;
}

SWidget* SWindow::GetChildrenAt(size_t IndexOf)
{
	//return Viewports[IndexOf];
	return nullptr;
}

void SWindow::OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry)
{
	InoutArrangedChildrens.AddWidget(_gameViewport->GetVisibility(), AllottedGeometry.MakeChild(
		_gameViewport,
		Vector2::Zero(),
		AllottedGeometry.GetLocalSize()
	));
}

DEFINE_SLATE_CONSTRUCTOR(SWindow, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);

	_device = Attr._RenderSystem->GetDevice();
	_commandQueue = Attr._RenderSystem->GetCommandQueue();
	_platformWindow = Attr._TargetWindow;

	_swapChain = _device->CreateSwapChain(_commandQueue, _platformWindow.Get());
	_gameViewport = SNew(SViewport)
		.Window(this);
}

void SWindow::TryResizeSwapChain(const Geometry& allottedGeometry)
{
	Vector2N size = Vector<>::Cast<int32>(allottedGeometry.GetLocalSize());
	if (_lastSwapChainSize != size)
	{
		EnqueueRenderThreadWork([self = SharedFromThis<SWindow>(), previous = _lastSwapChainSize, size]() mutable
		{
			self->_swapChain->ResizeBuffers(size);
			self->SwapChainResized.Broadcast(size);

			Log::Info(LogSlateWindow, TEXT("SwapChain resized from [{0}] to [{1}]."), previous, size);
		});

		_lastSwapChainSize = size;
	}
}