// Copyright 2020 Aumoa.lib. All right reserved.

#include "Engine.h"

#include "Application.h"
#include "GameInstance.h"
#include "GameViewport.h"
#include "RHI/IRHISwapChain.h"
#include "RHI/IRHIResource.h"
#include "Logging/LogVerbosity.h"
#include "Logging/LogMacros.h"
#include "SceneRendering/DeferredSceneRenderer.h"
#include "SceneRendering/Scene.h"
#include "SceneRendering/StaticMesh.h"
#include "RHI/IRHICommandFence.h"
#include "RHI/IRHIImmediateCommandList.h"
#include "RHI/IRHIRenderTargetView.h"
#include "RHI/IRHIDeferredCommandList.h"
#include "RHI/RHIShaderLibrary.h"
#include "RHI/RHIResourceStates.h"
#include "RHI/RHIResourceGC.h"
#include "RHI/RHIShaderDescription.h"
#include "RHI/IRHIShader.h"
#include "Assets/AssetManager.h"

#include "D3D12RHI/D3D12DeviceBundle.h"
//#include "VulkanRHI/VulkanDeviceBundle.h"

using namespace std;
using namespace std::chrono;

Engine* Engine::gEngine = nullptr;

Engine::Engine() : Super()
	, LogEngine(ELogVerbosity::Verbose, nameof(LogEngine))
	, gameInstance(nullptr)

	, deviceBundle(nullptr)
	, immediateCommandList(nullptr)
	, swapChain(nullptr)
{

}

Engine::~Engine()
{
	// Waiting for all GPU tasks.
	autoFence->BeginFence();
}

void Engine::Initialize()
{
	if (gEngine != nullptr)
	{
		SE_LOG(LogEngine, Fatal, L"Engine duplication detected.");
		throw Exception("Unexpected exception.");
	}
	gEngine = this;

	auto deviceBundle = NewObject<D3D12DeviceBundle>();
	this->deviceBundle = deviceBundle.Get();
	rhiBundles.push_back(deviceBundle);

	ForEachBundles([](auto* bundle) { bundle->InitializeBundle(); });

	autoFence = deviceBundle->CreateCommandFence();
	immediateCommandList = deviceBundle->GetImmediateCommandList();
	swapChain = deviceBundle->GetSwapChain().Get();

	sceneRenderer = NewObject<DeferredSceneRenderer>(this->deviceBundle);
	assetManager = NewObject<AssetManager>();
	gameViewport = NewObject<GameViewport>();
	GApplication.PostSized += bind_delegate(Application_OnPostSized);

	LoadEngineDefaultAssets();

	prev_tick = steady_clock::now();
}

void Engine::PostInitialize()
{
	gameInstance = GApplication.GetGameInstance();
	InitializeDefaultShaders();
}

void Engine::Tick()
{
	auto curr_tick = steady_clock::now();
	Seconds delta = Seconds(curr_tick - prev_tick);
	prev_tick = curr_tick;

	deviceBundle->GetResourceGC()->Collect();
	gameInstance->Tick(delta);

	TRefPtr<IRHISwapChain> swapChain = deviceBundle->GetSwapChain();

	autoFence->BeginFence();

	IRHIResource* target = swapChain->GetBuffer(swapChain->CurrentBackBufferIndex).Get();

	sceneRenderer->BeginRender();

	World* renderWorld = gameInstance->GetWorld();
	Scene* renderScene = renderWorld->GetScene();
	renderScene->Render(sceneRenderer.Get());
	sceneRenderer->PopulateRenderCommands();

	sceneRenderer->EndRender();
	immediateCommandList->ExecuteCommandList(sceneRenderer->CommandList);

	immediateCommandList->BeginCommand();
	immediateCommandList->ResourceTransition(target, ERHIResourceStates::PRESENT, ERHIResourceStates::COPY_DEST);
	immediateCommandList->CopyResource(target, sceneRenderer->FinalColor);
	immediateCommandList->ResourceTransition(target, ERHIResourceStates::COPY_DEST, ERHIResourceStates::PRESENT);
	immediateCommandList->EndCommand();
	immediateCommandList->Flush();

	swapChain->Present();

	autoFence->EndFence(immediateCommandList);
}

IRHIDeviceBundle* Engine::DeviceBundle_get() const
{
	return deviceBundle;
}

AssetManager* Engine::GetAssetManager() const
{
	return assetManager.Get();
}

GameViewport* Engine::GetGameViewport() const
{
	return gameViewport.Get();
}

Engine* Engine::GetInstance()
{
	return gEngine;
}

void Engine::ForEachBundles(function<void(IRHIBundle*)> action)
{
	for (auto& bundle : rhiBundles)
	{
		action(bundle.Get());
	}
}

void Engine::LoadEngineDefaultAssets()
{
	TRefPtr<StaticMesh> mesh = NewObject<StaticMesh>();
	GetAssetManager()->Import(L"Engine/StaticMesh/Triangle", mesh);
}

#include "Shaders/Compiled/VertexShader.hlsl.h"
#include "Shaders/Compiled/PixelShader.hlsl.h"

void Engine::InitializeDefaultShaders()
{
	RHIShaderLibrary* shaderLibrary = deviceBundle->GetShaderLibrary();

	RHIShaderDescription shaderDesc;
	shaderDesc.ShaderName = "Engine/Default";
	shaderDesc.VS = RHIShaderBytecode(pVertexShader);
	shaderDesc.PS = RHIShaderBytecode(pPixelShader);
	TRefPtr<IRHIShader> shader = DeviceBundle->CreateShader(shaderDesc);

	shaderLibrary->AddShader(shader);
}

void Engine::Application_OnPostSized(int32 x, int32 y)
{
	gameViewport->SetViewportResolution_Internal(x, y);
}