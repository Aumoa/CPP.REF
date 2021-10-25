// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Viewport.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "RHI/IRHITexture2D.h"
#include "RHI/IRHIDevice.h"
#include "GameThreads/RenderThread.h"

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

DEFINE_SLATE_CONSTRUCTOR(SViewport, InAttr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(InAttr);
	RenderSize = InAttr._RenderSize;
	RenderTargetFormat = InAttr._RenderTargetFormat;
	ReallocRenderTarget();
}

void SViewport::OnArrangeChildren(SArrangedChildrens* ArrangedChildrens, const Geometry& AllottedGeometry)
{
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
			TextureDesc.InitialState = ERHIResourceStates::CopySource;
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

			SE_LOG(LogViewport, Verbose, L"Viewport render targets are reallocated to [{}x{}].", RenderSize.X, RenderSize.Y);
		}
	}
}

void SViewport::DestroyRenderTarget_GameThread()
{
	struct
	{
		std::shared_ptr<SObject> RenderTargetHolder;
		std::shared_ptr<SObject> DepthStencilHolder;
	} Holder;

	if (RenderTarget != nullptr)
	{
		Holder.RenderTargetHolder = RenderTarget->shared_from_this();
		DestroyObject(RenderTarget);
		RenderTarget = nullptr;
	}

	if (DepthStencil != nullptr)
	{
		Holder.DepthStencilHolder = DepthStencil->shared_from_this();
		DestroyObject(DepthStencil);
		DepthStencil = nullptr;
	}

	// Finalize textures in render thread.
	RenderThread::EnqueueRenderThreadWork<"DestroyRenderTarget_RenderThread">([Holder = std::move(Holder)](){});
}