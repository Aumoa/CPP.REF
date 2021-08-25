// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "GameInstance.h"
#include "GameEngine.h"
#include "IFrameworkView.h"
#include "Level/World.h"
#include "Level/Level.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/LocalPlayer.h"

GameInstance::GameInstance() : Super()
{
}

void GameInstance::Tick(std::chrono::duration<float> elapsedTime)
{
	_world->LevelTick(elapsedTime);
	_localPlayer->Tick(elapsedTime);
}

int32 GameInstance::Run(IFrameworkView* frameworkView)
{
	_frameworkView = frameworkView;

	InitializeEngine();
	PostInitialized.Invoke();

	frameworkView->Start();
	return frameworkView->GetLastError();
}

IFrameworkView* GameInstance::GetFrameworkView() const
{
	return _frameworkView;
}

LocalPlayer* GameInstance::GetLocalPlayer() const
{
	return _localPlayer;
}

void GameInstance::InitializeEngine()
{
#ifdef _DEBUG
	constexpr bool bDebug = true;
#else
	constexpr bool bDebug = false;
#endif

	_localPlayer = CreateSubobject<LocalPlayer>();
	_localPlayer->Init(_engine);

	_world = CreateSubobject<World>(_engine);
	Level* loadedLevel = _world->LoadLevel(StartupLevel);
	loadedLevel->GetPlayerController()->_localPlayer = _localPlayer;

	SetWorld(_world);
}