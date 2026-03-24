// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Engine.h"
#include "Graphics.h"
#include "GenericApplication.h"
#include "GenericActivity.h"
#include "GenericWindowSwapchainExtension.h"
#include "GameInstance.h"
#include "CommandBuffer.h"
#include "TimerManager.h"
#include "Exceptions/ModuleNotFoundException.h"
#include "SceneManagement/Scene.h"
#include "SceneManagement/SceneManager.h"
#include "Threading/MainSynchronizationContext.h"
#include "Rendering/RenderThread.h"
#include "Rendering/SceneRenderer.h"
#include "Rendering/SceneView.h"
#include "Rendering/RenderTexture.h"
#include "Rendering/Camera.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderType.h"
#include "Rendering/RenderPipeline.h"
#include "Rendering/PositionColorVertexFactory.h"
#include "RenderPasses/GeometryRenderPass.h"
#include "IO/File.h"

namespace Ayla
{
	Engine::Engine()
	{
	}

	Engine::~Engine() noexcept
	{
	}

	void Engine::Initialize_Implementation()
	{
		m_MainActivity = GenericApplication::Get().CreateMainActivity();
		m_MainActivity->BeforeInitialize();

		m_Graphics = InitializeGraphics();
		m_RenderThread = New<RenderThread>(m_Graphics);
		m_CommandBuffer = m_Graphics->CreateCommandBuffer();
		m_SwapchainExtensions.emplace_back(m_Graphics->InstallSwapChain(m_MainActivity->GetMainWindow()));

		m_GameInstance = InitializeGameInstance();
		std::vector<Task<>> tasks;
		tasks.emplace_back(m_GameInstance->InitializeAsync({}));
		tasks.emplace_back(Task<>::Create([this]() -> Task<>
		{
			ShaderCreationInfo sci = {};
			sci.VertexFactory = std::make_shared<PositionColorVertexFactory>();
			std::vector<Task<>> tasks;
			tasks.emplace_back(File::ReadAllBytesAsync(TEXT("E:\\CPP.REF\\Engine\\Binaries\\Win64\\Debug\\Shaders\\DefaultVertex.cso")).ContinueWith([&](auto r)
			{
				auto& bytecode = r.GetResult();
				sci.VertexShader.Bytecode = std::move(bytecode);
				sci.VertexShader.EntrypointName = TEXT("main");
			}));
			tasks.emplace_back(File::ReadAllBytesAsync(TEXT("E:\\CPP.REF\\Engine\\Binaries\\Win64\\Debug\\Shaders\\DefaultPixel.cso")).ContinueWith([&](auto r)
			{
				auto& bytecode = r.GetResult();
				sci.FragmentShader.Bytecode = std::move(bytecode);
				sci.FragmentShader.EntrypointName = TEXT("main");
			}));

			co_await Task<>::WhenAll(tasks);
			auto shader = m_Graphics->CreateShader(std::move(sci));
			m_DefaultGeometryRenderPipeline = m_Graphics->CreateGeometryRenderPipeline(shader);
		}));
		tasks.emplace_back(Task<>::Create([this]() -> Task<>
		{
			ShaderCreationInfo sci = {};
			std::vector<Task<>> tasks;
			tasks.emplace_back(File::ReadAllBytesAsync(TEXT("E:\\CPP.REF\\Engine\\Binaries\\Win64\\Debug\\Shaders\\DefaultRayGeneration.cso")).ContinueWith([&](auto r)
			{
				auto& bytecode = r.GetResult();
				sci.RayGenerationShader.Bytecode = std::move(bytecode);
				sci.RayGenerationShader.EntrypointName = TEXT("DefaultRayGeneration");
			}));
			tasks.emplace_back(File::ReadAllBytesAsync(TEXT("E:\\CPP.REF\\Engine\\Binaries\\Win64\\Debug\\Shaders\\DefaultHit.cso")).ContinueWith([&](auto r)
			{
				auto& bytecode = r.GetResult();
				sci.ClosestHitShader.Bytecode = std::move(bytecode);
				sci.ClosestHitShader.EntrypointName = TEXT("DefaultClosestHit");
			}));
			tasks.emplace_back(File::ReadAllBytesAsync(TEXT("E:\\CPP.REF\\Engine\\Binaries\\Win64\\Debug\\Shaders\\DefaultMiss.cso")).ContinueWith([&](auto r)
			{
				auto& bytecode = r.GetResult();
				sci.MissShader.Bytecode = std::move(bytecode);
				sci.MissShader.EntrypointName = TEXT("DefaultMiss");
			}));

			co_await Task<>::WhenAll(tasks);
			auto shader = m_Graphics->CreateShader(std::move(sci));
			m_DefaultRaytracingRenderPipeline = m_Graphics->CreateRaytracingRenderPipeline(shader);
		}));

		Task<>::WhenAll(tasks).GetResult();

		m_MainActivity->AfterInitialize();
	}

	void Engine::GuardedLoop_Implementation()
	{
		auto& app = GenericApplication::Get();
		std::vector<GenericPlatformInputEvent> inputEvents;
		m_TimerManager = std::make_unique<TimerManager>();
		m_TimerManager->Start();
		
		m_TimerManager->AddInterval([this]()
		{
			auto dt = m_FrameTime / m_FrameCount;
			auto fps = 1.0 / dt;
			m_MainActivity->SetTitle(String::Format(TEXT("{}, FPS: {:.2f}"), m_Graphics->GetCurrentRenderFeature(), fps));
			m_FrameTime = 0;
			m_FrameCount = 0;
		}, TimeSpan::FromSeconds(1));

		while (true)
		{
			app.PumpMessages(inputEvents);
			if (app.IsQuitRequested())
			{
				break;
			}

			Tick();
		}

		m_GameInstance->Shutdown();
	}

	void Engine::Shutdown()
	{
		m_RenderThread->RequestStop();
		m_Graphics->WaitForCompletion();

		for (auto& swapchain : m_SwapchainExtensions)
		{
			swapchain->Destroy();
		}

		m_SwapchainExtensions.clear();

		m_DefaultGeometryRenderPipeline.Release();
		m_DefaultRaytracingRenderPipeline.Release();

		m_CommandBuffer->Dispose();
		m_Graphics->Dispose();
	}

	void Engine::Tick()
	{
		m_TimerManager->StartFrame();

		m_FrameTime += m_TimerManager->GetDeltaTime().GetTotalSeconds();
		m_FrameCount += 1;
		m_TimerManager->UpdateTasks();

		DispatchTick(TickTiming::Initialization);
		DispatchTick(TickTiming::TimeUpdate);
		DispatchTick(TickTiming::PreUpdate);
		DispatchTick(TickTiming::Update);
		DispatchTick(TickTiming::PostUpdate);
		DispatchTick(TickTiming::Physics);
		DispatchTick(TickTiming::PreLateUpdate);
		DispatchTick(TickTiming::LateUpdate);
		DispatchTick(TickTiming::PostLateUpdate);
		DispatchTick(TickTiming::PreRender);
		DispatchTick(TickTiming::PostRender);
		DispatchTick(TickTiming::EndOfFrame);

		m_Scratch.AllCameras.clear();
		m_GameInstance->GetSceneManager()->GetAllCameraComponents(&m_Scratch.AllCameras);
		m_Scratch.AllCameraViews.resize(m_Scratch.AllCameras.size());
		for (size_t i = 0; i < m_Scratch.AllCameras.size(); i++)
		{
			m_Scratch.AllCameras[i]->GetMinimalViewInfo(&m_Scratch.AllCameraViews[i]);
		}

		m_RenderThread->Dispatch([
			swapchainExtensions = m_SwapchainExtensions,
			graphics = m_Graphics,
			commandBuffer = m_CommandBuffer,
			self = m_RenderThread.Get(),
			renderPipeline = m_DefaultGeometryRenderPipeline.Get(),
			views = &m_Scratch.AllCameraViews
		]()
		{
			commandBuffer->WaitForCompletion(TimeSpan::FromSeconds(1));
			graphics->BeginRenderFrame();

			for (auto& swapchainExt : swapchainExtensions)
			{
				swapchainExt->DoResize();
			}

			self->ExecuteJobs();

			commandBuffer->BeginCommands();

			// Camera: Overlay, Display #0
			auto rt = swapchainExtensions[0]->GetRenderTexture();
			rt->Acquire(commandBuffer.Get());

			SceneRenderer renderer;

			GeometryRenderPass geometryPass(renderPipeline, rt.Get());
			renderer.AddPass(&geometryPass);

			for (auto& view : *views)
			{
				SceneView sceneView{ view };
				renderer.Render(commandBuffer.Get(), sceneView);
			}

			commandBuffer->EndCommands();

			for (auto& swapchainExt : swapchainExtensions)
			{
				swapchainExt->Present(commandBuffer.Get());
			}

			graphics->EndRenderFrame();
		});
	}

	SharedPtr<Graphics> Engine::GetGraphics() const
	{
		return m_Graphics;
	}

	SharedPtr<Graphics> Engine::InitializeGraphics_Implementation()
	{
		fail();
	}

	SharedPtr<GameInstance> Engine::InitializeGameInstance_Implementation()
	{
		fail();
	}

	void Engine::DispatchTick(TickTiming timing)
	{
		m_GameInstance->Tick(timing, m_TimerManager->GetDeltaTime());
	}
}