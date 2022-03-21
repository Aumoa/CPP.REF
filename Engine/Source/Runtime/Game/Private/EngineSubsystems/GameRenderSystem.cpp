// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameRenderSystem.h"
#include "RHI/RHIInterfaces.h"
#include "ShaderWorkspaces/ColorShaderWorkspace.h"

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

	ShaderCodeWorkspace = gcnew SColorShaderWorkspace(Device);
	ShaderCodeWorkspace->Compile();

	IApplicationInterface::Get().Sized.AddObject(this, &SGameRenderSystem::ResizeApp);
	IApplicationInterface::Get().PreDestroyApp.AddObject(this, &SGameRenderSystem::OnPreDestroyApp);
}

void SGameRenderSystem::Deinit()
{
	RenderThread.reset();

	if (CommandFence)
	{
		CommandFence->SetEventOnCompletion(_fenceValue, std::nullopt).Wait();
		CommandFence->Dispose();
		CommandFence = nullptr;
	}

	if (ShaderCodeWorkspace)
	{
		ShaderCodeWorkspace->Dispose();
		ShaderCodeWorkspace = nullptr;
	}

	if (RTV)
	{
		RTV->Dispose();
		RTV = nullptr;
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
	RenderThread::Get()->ExecuteWorks(nullptr, std::bind(&SGameRenderSystem::OnRender_RenderThread, this, std::placeholders::_1));
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
		RenderThread::Get()->ExecuteWorks(nullptr, std::bind(&SGameRenderSystem::OnResizeApp_RenderThread,
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

void SGameRenderSystem::OnRender_RenderThread(IRHIGraphicsCommandList* Cmdbuf)
{
	//int32 CurrentIndex = PrimaryQueue->AcquireSwapChainImage(SwapChain);
	//RectN ClearArea = RectN(0, 0, SwapChainSize.X, SwapChainSize.Y);

	//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	//CommandAllocator->Reset();

	//CommandBuffer->BeginRecord();
	//{
	//	RHIResourceTransitionBarrier ToRenderTarget =
	//	{
	//		.pResource = SwapChain->GetBuffer(CurrentIndex),
	//		.StateBefore = ERHIResourceStates::Present,
	//		.StateAfter = ERHIResourceStates::RenderTarget,
	//	};
	//	CommandBuffer->ResourceBarrier(std::vector{ RHIResourceBarrier(ToRenderTarget) });

	//	CommandBuffer->BeginRenderPass(RenderPasses[CurrentIndex], ClearArea, std::vector{ NamedColors::Transparent });
	//	CommandBuffer->EndRenderPass();
	//}
	//CommandBuffer->EndRecord();

	//PrimaryQueue->Submit(CommandFence, CommandBuffer);
	//PrimaryQueue->Present(SwapChain, CurrentIndex);

	//CommandFence->Wait();
	//SwapChain->Wait();
}

void SGameRenderSystem::OnResizeApp_RenderThread(IRHIGraphicsCommandList* Cmdbuf, Vector2N Size)
{
	//SwapChain->ResizeBuffers(Size);
	//SwapChainSize = Size;

	//for (int32 i = 0; i < 3; ++i)
	//{
	//	RHIRenderTargetViewDesc RTVDesc =
	//	{
	//		.Format = ERHIPixelFormat::B8G8R8A8_UNORM,
	//		.ViewDimension = ERHIRTVDimension::Texture2D,
	//	};

	//	IRHITexture2D* Buffer = SwapChain->GetBuffer(i);
	//	RTV->CreateRenderTargetView(i, Buffer, &RTVDesc);

	//	RenderPasses[i]->Apply(std::vector{ std::make_pair((IRHIView*)RTV, i) }, Size);
	//}
}