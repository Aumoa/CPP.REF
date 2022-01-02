// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Application/Viewport.h"
#include "RHI/IRHITexture2D.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIRenderTargetView.h"
#include "RHI/IRHIDepthStencilView.h"
#include "RHI/IRHIShaderResourceView.h"
#include "RHI/IRHIDeviceContext.h"
#include "Draw/PaintArgs.h"
#include "Draw/SlateDrawElement.h"
#include "IApplicationInterface.h"
#include "Level/World.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "GameEngine.h"

GENERATE_BODY(SViewport);
DEFINE_LOG_CATEGORY(LogViewport);

SViewport::SViewport() : Super()
{
}

SViewport::~SViewport()
{
	Widgets.clear();
	DestroyRenderTarget_GameThread();
}

void SViewport::Tick(const Geometry& AllottedGeometry, float InDeltaTime)
{
	Super::Tick(AllottedGeometry, InDeltaTime);

	Vector2N LocalSize = AllottedGeometry.GetLocalSize().Cast<int32>();
	SetRenderSize(LocalSize);
}

void SViewport::SetRenderSize(Vector2N InRenderSize)
{
	if (RenderSize != InRenderSize)
	{
		RenderSize = InRenderSize;
		ReallocRenderTarget();
	}
}

Vector2N SViewport::GetRenderSize()
{
	return RenderSize;
}

void SViewport::PopulateCommandLists(IRHIDeviceContext* InDeviceContext)
{
}

Vector2 SViewport::GetDesiredSize()
{
	return RenderSize.Cast<float>();
}

void SViewport::AddToViewport(SWidget* InWidget)
{
	Widgets.emplace_back(InWidget);
}

void SViewport::RemoveFromViewport(SWidget* InWidget)
{
	for (auto It = Widgets.begin(); It != Widgets.end(); ++It)
	{
		if (*It == InWidget)
		{
			Widgets.erase(It);
			break;
		}
	}
}

DEFINE_SLATE_CONSTRUCTOR(SViewport, InAttr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(InAttr);
	RenderSize = InAttr._RenderSize;
	RenderTargetFormat = InAttr._RenderTargetFormat;
	ReallocRenderTarget();
}

void SViewport::OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry)
{
	for (auto& Widget : Widgets)
	{
		ESlateVisibility Visibility = Widget->GetVisibility();
		if (ArrangedChildrens.Accepts(Visibility))
		{
			ArrangedChildrens.AddWidget(Visibility, AllottedGeometry.MakeChild(
				Widget,
				Vector2(0.0f, 0.0f),
				AllottedGeometry.GetLocalSize()
			));
		}
	}
}

int32 SViewport::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled)
{
	//SlateBrush Brush;
	//Brush.ImageSource = SRV;
	//Brush.ImageSize = RenderSize.Cast<float>();
	//SlateDrawElement::MakeBox(InDrawElements, Brush, AllottedGeometry.ToPaintGeometry(), InLayer);

	return Super::OnPaint(Args, AllottedGeometry, CullingRect, InDrawElements, InLayer + 1, bParentEnabled);
}

void SViewport::ReallocRenderTarget()
{
	DestroyRenderTarget_GameThread();

	auto RenderSystem = GEngine->GetEngineSubsystem<SGameRenderSystem>();
	if (RenderSystem)
	{
	//	if (ensure(RenderSize.X != 0) && ensure(RenderSize.Y != 0))
	//	{
	//		IRHIDevice* Device = RenderSystem->GetRHIDevice();

	//		RHITexture2DDesc TextureDesc = {};
	//		TextureDesc.Width = (uint32)RenderSize.X;
	//		TextureDesc.Height = (uint32)RenderSize.Y;
	//		TextureDesc.DepthOrArraySize = 1;
	//		TextureDesc.MipLevels = 1;
	//		TextureDesc.Format = RenderTargetFormat;
	//		TextureDesc.Usage = ERHIBufferUsage::Default;
	//		TextureDesc.Flags = ERHIResourceFlags::AllowRenderTarget;
	//		TextureDesc.InitialState = ERHIResourceStates::PixelShaderResource;
	//		TextureDesc.ClearValue.emplace() =
	//		{
	//			.Format = TextureDesc.Format,
	//			.ClearColor = {}
	//		};
	//		RenderTarget = Device->CreateTexture2D(TextureDesc, nullptr);
	//		RenderTarget->SetOuter(this);
	//		RenderTarget->SetDebugName(L"SViewport: RenderTargetTexture");

	//		TextureDesc.Format = ERHIPixelFormat::D24_UNORM_S8_UINT;
	//		TextureDesc.Flags = ERHIResourceFlags::AllowDepthStencil;
	//		TextureDesc.InitialState = ERHIResourceStates::DepthWrite;
	//		TextureDesc.ClearValue.emplace() =
	//		{
	//			.Format = TextureDesc.Format,
	//			.DepthStencil =
	//			{
	//				.Depth = 1.0f,
	//				.Stencil = 0
	//			}
	//		};
	//		DepthStencil = Device->CreateTexture2D(TextureDesc, nullptr);
	//		DepthStencil->SetOuter(this);
	//		DepthStencil->SetDebugName(L"SViewport: DepthStencilTexture");

	//		RTV = Device->CreateRenderTargetView(1);
	//		RTV->SetOuter(this);
	//		DSV = Device->CreateDepthStencilView(1);
	//		DSV->SetOuter(this);
	//		SRV = Device->CreateShaderResourceView(1);
	//		SRV->SetOuter(this);

	//		RTV->CreateRenderTargetView(0, RenderTarget, nullptr);
	//		DSV->CreateDepthStencilView(0, DepthStencil, nullptr);
	//		SRV->CreateShaderResourceView(0, RenderTarget, nullptr);

	//		if (DeviceContext == nullptr)
	//		{
	//			DeviceContext = Device->CreateDeviceContext();
	//		}

			SE_LOG(LogViewport, Verbose, L"Viewport render targets are reallocated to [{}x{}].", RenderSize.X, RenderSize.Y);
	//	}
	}
}

void SViewport::DestroyRenderTarget_GameThread()
{
	//auto MoveTemp = [](auto*& InObject) -> SharedPtr<SObject>
	//{
	//	SharedPtr Object = InObject;
	//	InObject = nullptr;
	//	return Object;
	//};

	//std::vector<SharedPtr<SObject>> Holders = { MoveTemp(RTV), MoveTemp(DSV), MoveTemp(SRV), MoveTemp(RenderTarget), MoveTemp(DepthStencil) };

	//// Finalize textures in render thread.
	////RenderThread::EnqueueRenderThreadWork<"DestroyRenderTarget_RenderThread">([Holder = Holders](auto){});
}