// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameRenderSystem.h"
#include "RHI/RHIInterfaces.h"

GENERATE_BODY(SGameRenderSystem);
DEFINE_LOG_CATEGORY(LogRender);

SGameRenderSystem::SGameRenderSystem() : Super()
{
}

SGameRenderSystem::~SGameRenderSystem()
{
}

void SGameRenderSystem::Init()
{
	RenderThread.emplace();

	Factory = IApplicationInterface::Get().GetFactory();
	IRHIAdapter* PrimaryAdapter = Factory->GetAdapter(0);
	Device = Factory->CreateDevice(PrimaryAdapter);
	PrimaryQueue = Device->CreateCommandQueue();

	SwapChain = Factory->CreateSwapChain(PrimaryQueue, 3);

	CommandAllocator = Device->CreateCommandAllocator();
	CommandBuffer = Device->CreateCommandBuffer(CommandAllocator);
	CommandFence = Device->CreateFence();

	RTV = Device->CreateRenderTargetView(3);
	RenderPasses.resize(3);

	ShaderCodeWorkspace = Device->CreateShaderCodeWorkspace();

	for (auto& RenderPass : RenderPasses)
	{
		RenderPass = Device->CreateRenderPass();
		RenderPass->AddColorAttachment(RHIRenderPassAttachment
		{
			.Format = ERHIPixelFormat::B8G8R8A8_UNORM,
			.SampleDesc = { 1, 0 },
			.LoadOp = ERHIAttachmentLoadOp::Clear,
			.StoreOp = ERHIAttachmentStoreOp::Store,
			.BeforeState = ERHIResourceStates::RenderTarget,
			.AfterState = ERHIResourceStates::Present,
		});
	}

	IApplicationInterface::Get().Sized.AddSObject(this, &SGameRenderSystem::ResizeApp);
	IApplicationInterface::Get().PreDestroyApp.AddSObject(this, &SGameRenderSystem::OnPreDestroyApp);
}

void SGameRenderSystem::Deinit()
{
	RenderThread.reset();

	if (CommandFence)
	{
		CommandFence->Wait();
		CommandFence->Dispose();
		CommandFence = nullptr;
	}

	if (ShaderCodeWorkspace)
	{
		ShaderCodeWorkspace->Dispose();
		ShaderCodeWorkspace = nullptr;
	}

	for (auto& RenderPass : RenderPasses)
	{
		if (RenderPass)
		{
			RenderPass->Dispose();
			RenderPass = nullptr;
		}
	}

	if (RTV)
	{
		RTV->Dispose();
		RTV = nullptr;
	}

	if (CommandBuffer)
	{
		CommandBuffer->Dispose();
		CommandBuffer = nullptr;
	}

	if (CommandAllocator)
	{
		CommandAllocator->Dispose();
		CommandAllocator = nullptr;
	}

	if (SwapChain)
	{
		SwapChain->Dispose();
		SwapChain = nullptr;
	}

	if (PrimaryQueue)
	{
		PrimaryQueue->Dispose();
		PrimaryQueue = nullptr;
	}

	if (Device)
	{
		Device->Dispose();
		Device = nullptr;
	}

	if (Factory)
	{
		Factory->Dispose();
		Factory = nullptr;
	}

	Super::Deinit();
}

void SGameRenderSystem::ExecuteRenderThread(float InDeltaTime, SSlateApplication* SlateApp)
{
	RenderThread::Get()->ExecuteWorks(CommandBuffer, std::bind(&SGameRenderSystem::OnRender_RenderThread, this, std::placeholders::_1));
}

IRHIDevice* SGameRenderSystem::GetRHIDevice()
{
	return Device;
}

void SGameRenderSystem::ResizeApp(Vector2N Size)
{
	if (Size.Size() == 0)
	{
		return;
	}

	if (Device)
	{
		RenderThread::Get()->ExecuteWorks(CommandBuffer, std::bind(&SGameRenderSystem::OnResizeApp_RenderThread,
			this,
			std::placeholders::_1,
			Size
		));
	}
}

void SGameRenderSystem::OnPreDestroyApp()
{
	RenderThread.reset();
}

void SGameRenderSystem::OnRender_RenderThread(IRHICommandBuffer* Cmdbuf)
{
	int32 CurrentIndex = PrimaryQueue->AcquireSwapChainImage(SwapChain);
	RectN ClearArea = RectN(0, 0, SwapChainSize.X, SwapChainSize.Y);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	CommandAllocator->Reset();

	CommandBuffer->BeginRecord();
	RHIResourceTransitionBarrier ToRenderTarget =
	{
		.pResource = SwapChain->GetBuffer(CurrentIndex),
		.StateBefore = ERHIResourceStates::Present,
		.StateAfter = ERHIResourceStates::RenderTarget,
	};
	CommandBuffer->ResourceBarrier(std::vector{ RHIResourceBarrier(ToRenderTarget) });

	CommandBuffer->BeginRenderPass(RenderPasses[CurrentIndex], ClearArea, std::vector{ NamedColors::Red });
	CommandBuffer->EndRenderPass();
	CommandBuffer->EndRecord();

	PrimaryQueue->Submit(CommandFence, CommandBuffer);
	PrimaryQueue->Present(SwapChain, CurrentIndex);

	CommandFence->Wait();
	SwapChain->Wait();
}

void SGameRenderSystem::OnResizeApp_RenderThread(IRHICommandBuffer* Cmdbuf, Vector2N Size)
{
	SwapChain->ResizeBuffers(Size);
	SwapChainSize = Size;

	for (int32 i = 0; i < 3; ++i)
	{
		RHIRenderTargetViewDesc RTVDesc =
		{
			.Format = ERHIPixelFormat::B8G8R8A8_UNORM,
			.ViewDimension = ERHIRTVDimension::Texture2D,
		};

		IRHITexture2D* Buffer = SwapChain->GetBuffer(i);
		RTV->CreateRenderTargetView(i, Buffer, &RTVDesc);

		RenderPasses[i]->Apply(std::vector{ std::make_pair((IRHIView*)RTV, i) }, Size);
	}
}