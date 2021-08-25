// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "GameEngine.h"
#include "LogGame.h"
#include "GameInstance.h"
#include "IFrameworkView.h"
#include "Shaders/ColorShader/ColorVertexFactory.h"
#include "Shaders/ColorShader/ColorShader.h"
#include "Shaders/TransparentShader/TransparentShader.h"
#include "Shaders/SlateShader/SlateShader.h"
#include "Components/InputComponent.h"
#include "Level/World.h"
#include "Camera/PlayerCameraManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneRenderer.h"
#include "Assets/AssetImporter.h"
#include "GameFramework/LocalPlayer.h"
#include "Misc/CommandLine.h"
#include "PlatformMisc/PlatformModule.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "EngineSubsystems/GameModuleSystem.h"
#include "EngineSubsystems/GameAssetSystem.h"

GameEngine* GameEngine::_gEngine = nullptr;

GameEngine::GameEngine() : Super()
{
}

GameEngine::~GameEngine()
{
}

bool GameEngine::InitEngine()
{
	_gEngine = this;
	InitializeSubsystems();
	return true;
}

void GameEngine::SetupFrameworkView(IFrameworkView* frameworkView)
{
	GetEngineSubsystem<GameRenderSystem>()->SetupFrameworkView(frameworkView);

	SE_LOG(LogEngine, Info, L"Register engine tick.");
	frameworkView->Idle += [this]() { TickEngine(); };
	frameworkView->Size += [this](int32 width, int32 height) { ResizedApp(width, height); };
}

bool GameEngine::LoadGameModule(std::wstring_view moduleName)
{
	GameModuleSystem* system = GetEngineSubsystem<GameModuleSystem>();
	system->LoadGameModule(moduleName);
	if (_gameInstance = system->LoadGameInstance(); _gameInstance == nullptr)
	{
		SE_LOG(LogEngine, Fatal, L"LoadGameInstance function from {} module return nullptr.", moduleName);
		return false;
	}
	return true;
}

int32 GameEngine::InvokedMain(IFrameworkView* frameworkView, std::wstring_view platformArgs)
{
	CommandLine commandArgs = StringUtils::Split(platformArgs, L" ", true, true);
	size_t gameModuleIdx = commandArgs.GetArgument(L"--GameDll");
	if (gameModuleIdx == -1)
	{
		SE_LOG(LogEngine, Fatal, L"GameModule does not specified.");
		return -1;
	}

	std::optional<std::wstring_view> moduleName = commandArgs.GetArgument(gameModuleIdx + 1);
	if (!moduleName)
	{
		SE_LOG(LogEngine, Fatal, L"GameModule does not specified.");
		return -1;
	}

	// Create GameEngine instance and initialize it.
	std::unique_ptr engine = std::make_unique<GameEngine>();
	if (!engine->InitEngine())
	{
		return -1;
	}

	// Load game module.
	if (!engine->LoadGameModule(*moduleName))
	{
		return -1;
	}

	return 0;
}

GameInstance* GameEngine::GetGameInstance() const
{
	return _gameInstance;
}

void GameEngine::InitializeSubsystems()
{
	SE_LOG(LogEngine, Verbose, L"Initialize subsystems.");
	_subsystems.emplace_back(CreateSubobject<GameRenderSystem>())->Init();
	_subsystems.emplace_back(CreateSubobject<GameAssetSystem>())->Init();
	_subsystems.emplace_back(CreateSubobject<GameModuleSystem>())->Init();
}

void GameEngine::TickEngine()
{
	using namespace std::chrono;

	duration<float> deltaSeconds = 0ns;
	steady_clock::time_point now = steady_clock::now();
	if (_prev.has_value())
	{
		deltaSeconds = now - _prev.value();
	}
	_prev = now;

	GameTick(deltaSeconds);
	RenderTick(deltaSeconds);
}

void GameEngine::ResizedApp(int32 width, int32 height)
{
	if (width == 0 || height == 0)
	{
		return;
	}

	// On the framework view is resized, wait all graphics commands for
	// synchronize and cleanup resource lock states.
	//_primaryQueue->WaitLastSignal();

	//_frameworkViewChain->ResizeBuffers(width, height);

	//for (int32 i = 0; i < 3; ++i)
	//{
	//	RHITexture2D* texture = _frameworkViewChain->GetBuffer(i);
	//	_rtv->CreateRenderTargetView(texture, i);
	//}

	// Resize depth stencil buffer.
	//if (_depthBuffer != nullptr)
	//{
	//	DestroySubobject(_depthBuffer);
	//}

	//RHITexture2DClearValue clearValue =
	//{
	//	.Format = ERHIPixelFormat::D24_UNORM_S8_UINT,
	//	.DepthStencil = { 1.0f, 0 }
	//};

	//_depthBuffer = _device->CreateTexture2D(ERHIResourceStates::DepthWrite, ERHIPixelFormat::D24_UNORM_S8_UINT, width, height, clearValue, ERHIResourceFlags::AllowDepthStencil);
	//_dsv->CreateDepthStencilView(_depthBuffer, 0);

	//_vpWidth = width;
	//_vpHeight = height;

	SE_LOG(LogEngine, Info, L"Application resized to {}x{}.", width, height);
}

void GameEngine::GameTick(std::chrono::duration<float> elapsedTime)
{
	// Update input.
	InputComponent::StaticTick(elapsedTime);

	_gameInstance->Tick(elapsedTime);
}

void GameEngine::RenderTick(std::chrono::duration<float> elapsedTime)
{
	//int32 bufferIdx = _frameworkViewChain->GetCurrentBackBufferIndex();

	//RHIViewport vp =
	//{
	//	.TopLeftX = 0,
	//	.TopLeftY = 0,
	//	.Width = (float)_vpWidth,
	//	.Height = (float)_vpHeight,
	//	.MinDepth = 0,
	//	.MaxDepth = 1.0f
	//};

	//RHIScissorRect sc =
	//{
	//	.Left = 0,
	//	.Top = 0,
	//	.Right = _vpWidth,
	//	.Bottom = _vpHeight
	//};

	//RHITransitionBarrier barrierBegin =
	//{
	//	.Resource = _frameworkViewChain->GetBuffer(bufferIdx),
	//	.StateBefore = ERHIResourceStates::Present,
	//	.StateAfter = ERHIResourceStates::RenderTarget
	//};
	//RHITransitionBarrier barrierEnd =
	//{
	//	.Resource = _frameworkViewChain->GetBuffer(bufferIdx),
	//	.StateBefore = ERHIResourceStates::RenderTarget,
	//	.StateAfter = ERHIResourceStates::Present
	//};

	//_deviceContext->Begin();
	//_deviceContext->TransitionBarrier(1, &barrierBegin);
	//_deviceContext->OMSetRenderTargets(_rtv, bufferIdx, 1, _dsv, 0);
	//_deviceContext->ClearRenderTargetView(_rtv, bufferIdx, NamedColors::Transparent);
	//_deviceContext->ClearDepthStencilView(_dsv, 0, 1.0f, std::nullopt);
	//_deviceContext->RSSetScissorRects(1, &sc);
	//_deviceContext->RSSetViewports(1, &vp);

	//World* world = _gameInstance->GetWorld();
	//APlayerCameraManager* playerCamera = world->GetPlayerCamera();

	//Scene* scene = _gameInstance->GetWorld()->GetScene();
	//MinimalViewInfo localPlayerView = playerCamera->GetCachedCameraView();
	//localPlayerView.AspectRatio = (float)_vpWidth / (float)_vpHeight;
	//scene->InitViews(localPlayerView);
	//scene->RenderScene(_deviceContext);

	//LocalPlayer* localPlayer = _gameInstance->GetLocalPlayer();
	//if (localPlayer)
	//{
	//	_deviceContext->SetGraphicsShader(_slateShader);
	//	_deviceContext->IASetPrimitiveTopology(ERHIPrimitiveTopology::TriangleStrip);
	//	localPlayer->Render(_deviceContext, _slateShader);
	//}

	//_deviceContext->TransitionBarrier(1, &barrierEnd);
	//_deviceContext->End();

	//_primaryQueue->ExecuteDeviceContext(_deviceContext);

	//_frameworkViewChain->Present();
	//_primaryQueue->Signal();
	//_primaryQueue->WaitLastSignal();
}