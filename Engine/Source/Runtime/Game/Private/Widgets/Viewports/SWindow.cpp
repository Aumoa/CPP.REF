// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/Viewports/SWindow.h"
#include "Widgets/Viewports/SViewport.h"
#include "Widgets/Drawing/PaintArgs.h"
#include "RHI/RHISwapChain.h"
#include "RHI/RHIDevice.h"
#include "RHI/RHIRenderThread.h"
#include "RHI/RHICommandList.h"
#include "RHI/RHICommandQueue.h"
#include "RHI/RHIStructures.h"
#include "EngineCore/GameRenderSubsystem.h"
#include "PlatformMisc/IPlatformWindow.h"
#include "SWindow.gen.cpp"

constexpr LogCategory LogSlateWindow = TEXT("LogSlateWindow");

SWindow::SWindow()
	: Super()
{
}

SWindow::~SWindow() noexcept
{
}

void SWindow::Tick(const Geometry& AllottedGeometry, const TimeSpan& DeltaTime)
{
	Super::Tick(AllottedGeometry, DeltaTime);
	TryResizeSwapChain(AllottedGeometry);
}

void SWindow::DispatchSlateTick(const TimeSpan& DeltaTime)
{
	Vector2N DrawingSize = PlatformWindow->GetDrawingSize();
	auto AllottedGeometry = Geometry::MakeRoot(
		Vector<>::Cast<Vector2>(DrawingSize),
		SlateLayoutTransform::Identity()
	);
	Tick(AllottedGeometry, DeltaTime);

	CachedDrawingSize = DrawingSize;
	CachedDeltaTime = DeltaTime;
}

void SWindow::PresentWindow(SceneRenderContext& Context)
{
	auto AllottedGeometry = Geometry::MakeRoot(
		Vector<>::Cast<Vector2>(CachedDrawingSize),
		SlateLayoutTransform::Identity(),
		SlateRenderTransform::Identity()
	);

	// Starting paint.
	PaintArgs Args = PaintArgs::NewArgs(SharedFromThis(), CachedDeltaTime, WindowCmdList.get());
	Paint(Args, AllottedGeometry, Rect(0.0f, 0.0f, (float)CachedDrawingSize.X, (float)CachedDrawingSize.Y), nullptr, 0, true);
	
	WindowCmdList->BeginFrame();

	std::shared_ptr<RHIResource> ColorSurface = GameViewport->PresentViewport(Context, WindowCmdList.get());

	size_t Index = SwapChain->GetCurrentBackBufferIndex();
	auto Surface = SwapChain->GetBuffer(Index);

	RHIResourceBarrier Barrier =
	{
		.Type = ERHIResourceBarrierType::Transition,
		.Flags = ERHIResourceBarrierFlags::None,
		.Transition =
		{
			.pResource = Surface.get(),
			.Subresource = 0,
			.StateBefore = ERHIResourceStates::Present,
			.StateAfter = ERHIResourceStates::CopyDest
		}
	};

	WindowCmdList->ResourceBarrier({ &Barrier, 1 });
	WindowCmdList->CopyResource(Surface.get(), ColorSurface.get());

	std::swap(Barrier.Transition.StateBefore, Barrier.Transition.StateAfter);
	WindowCmdList->ResourceBarrier({ &Barrier, 1 });

	WindowCmdList->EndFrame();

	auto Cmds = std::vector{ WindowCmdList };
	CommandQueue->ExecuteCommandLists(Cmds);
	SwapChain->Present();
}

size_t SWindow::NumChildrens()
{
	return 1;
}

SWidget* SWindow::GetChildrenAt(size_t IndexOf)
{
	check(IndexOf == 0);
	return GameViewport.get();
}

std::shared_ptr<RHIDevice> SWindow::GetDevice() const
{
	return Device;
}

void SWindow::OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry)
{
	InoutArrangedChildrens.AddWidget(GameViewport->GetVisibility(), AllottedGeometry.MakeChild(
		GameViewport,
		Vector2::Zero(),
		AllottedGeometry.GetLocalSize()
	));
}

DEFINE_SLATE_CONSTRUCTOR(SWindow, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);

	Device = Attr._RenderSystem->GetDevice();
	CommandQueue = Attr._RenderSystem->GetCommandQueue();
	PlatformWindow = Attr._TargetWindow;

	SwapChain = Device->CreateSwapChain(CommandQueue, PlatformWindow.Get());
	GameViewport = SNew(SViewport)
		.Window(this);

	WindowCmdList = Device->CreateCommandList();
}

void SWindow::TryResizeSwapChain(const Geometry& AllottedGeometry)
{
	Vector2N size = Vector<>::Cast<int32>(AllottedGeometry.GetLocalSize());
	if (LastSwapChainSize != size)
	{
		EnqueueRenderThreadWork([self = SharedFromThis<SWindow>(), previous = LastSwapChainSize, size]() mutable
		{
			self->SwapChain->ResizeBuffers(size);
			self->SwapChainResized.Broadcast(size);

			Log::Info(LogSlateWindow, TEXT("SwapChain resized from [{0}] to [{1}]."), previous, size);
		});

		LastSwapChainSize = size;
	}
}