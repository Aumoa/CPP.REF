// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Viewport.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "RHI/IRHITexture2D.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIRenderTargetView.h"
#include "RHI/IRHIDepthStencilView.h"
#include "RHI/IRHIShaderResourceView.h"
#include "GameThreads/RenderThread.h"
#include "Draw/PaintArgs.h"
#include "Draw/SlateDrawElement.h"
#include "Level/World.h"
#include "SceneRendering/ForwardSceneRenderer.h"

DEFINE_LOG_CATEGORY(LogViewport);

SViewport::SViewport() : Super()
{
}

SViewport::~SViewport()
{
	DestroyRenderTarget_GameThread();
}

void SViewport::SetRenderSize(const Vector2N& InRenderSize)
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
	if (GameWorld)
	{
		SceneRenderTarget RenderTarget(RTV, 0, DSV, 0, ERHIResourceStates::PixelShaderResource);
		// TODO: Render world.
		ensure(false);
	}

	SceneRenderTarget RenderTarget(RTV, 0, DSV, 0, ERHIResourceStates::PixelShaderResource);
	ForwardSceneRenderer ForwardRenderer(RenderTarget, nullptr);
	ForwardRenderer.PopulateCommandLists(InDeviceContext);
}

Vector2 SViewport::GetDesiredSize()
{
	return RenderSize.Cast<float>();
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
}

int32 SViewport::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled)
{
	PopulateCommandLists(Args.DeviceContext);

	SlateBrush Brush;
	Brush.ImageSource = SRV;
	Brush.ImageSize = RenderSize.Cast<float>();
	SlateDrawElement::MakeBox(InDrawElements, Brush, AllottedGeometry.ToPaintGeometry(), InLayer);

	return Super::OnPaint(Args, AllottedGeometry, CullingRect, InDrawElements, InLayer + 1, bParentEnabled);
}

void SViewport::ReallocRenderTarget()
{
	DestroyRenderTarget_GameThread();

	auto RenderSystem = GEngine->GetEngineSubsystem<SGameRenderSystem>();
	if (RenderSystem)
	{
		if (ensure(RenderSize.X != 0) && ensure(RenderSize.Y != 0))
		{
			IRHIDevice* Device = RenderSystem->GetRHIDevice();

			RHITexture2DDesc TextureDesc = {};
			TextureDesc.Width = (uint32)RenderSize.X;
			TextureDesc.Height = (uint32)RenderSize.Y;
			TextureDesc.DepthOrArraySize = 1;
			TextureDesc.MipLevels = 1;
			TextureDesc.Format = RenderTargetFormat;
			TextureDesc.Usage = ERHIBufferUsage::Default;
			TextureDesc.Flags = ERHIResourceFlags::AllowRenderTarget;
			TextureDesc.InitialState = ERHIResourceStates::PixelShaderResource;
			TextureDesc.ClearValue.emplace() =
			{
				.Format = TextureDesc.Format,
				.ClearColor = {}
			};
			RenderTarget = Device->CreateTexture2D(TextureDesc, nullptr);
			RenderTarget->SetOuter(this);
			RenderTarget->SetDebugName(L"SViewport: RenderTargetTexture");

			TextureDesc.Format = ERHIPixelFormat::D24_UNORM_S8_UINT;
			TextureDesc.Flags = ERHIResourceFlags::AllowDepthStencil;
			TextureDesc.InitialState = ERHIResourceStates::DepthWrite;
			TextureDesc.ClearValue.emplace() =
			{
				.Format = TextureDesc.Format,
				.DepthStencil =
				{
					.Depth = 1.0f,
					.Stencil = 0
				}
			};
			DepthStencil = Device->CreateTexture2D(TextureDesc, nullptr);
			DepthStencil->SetOuter(this);
			DepthStencil->SetDebugName(L"SViewport: DepthStencilTexture");

			RTV = Device->CreateRenderTargetView(1);
			RTV->SetOuter(this);
			DSV = Device->CreateDepthStencilView(1);
			DSV->SetOuter(this);
			SRV = Device->CreateShaderResourceView(1);
			SRV->SetOuter(this);

			RTV->CreateRenderTargetView(0, RenderTarget, nullptr);
			DSV->CreateDepthStencilView(0, DepthStencil, nullptr);
			SRV->CreateShaderResourceView(0, RenderTarget, nullptr);

			SE_LOG(LogViewport, Verbose, L"Viewport render targets are reallocated to [{}x{}].", RenderSize.X, RenderSize.Y);
		}
	}
}

void SViewport::DestroyRenderTarget_GameThread()
{
	auto MoveTemp = [](auto*& InObject)
	{
		SObject* Object = InObject;
		InObject = nullptr;
		return Object;
	};

	std::vector<std::shared_ptr<SObject>> Holders;
	SObject* Objects[] = { MoveTemp(RTV), MoveTemp(DSV), MoveTemp(SRV), MoveTemp(RenderTarget), MoveTemp(DepthStencil) };
	Holders.reserve(std::size(Objects));

	for (auto& Object : Objects)
	{
		if (Object)
		{
			Holders.emplace_back(Object->shared_from_this());
			DestroyObject(Object);
		}
	}

	// Finalize textures in render thread.
	RenderThread::EnqueueRenderThreadWork<"DestroyRenderTarget_RenderThread">([Holder = Holders](){});
}