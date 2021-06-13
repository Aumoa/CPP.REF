// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Core;
import SC.Runtime.Game;

using namespace std;
using namespace std::chrono;

GameInstance::GameInstance() : Super()
{
}

void GameInstance::Tick(duration<float> elapsedTime)
{
	_world->LevelTick(elapsedTime);
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

void GameInstance::InitializeEngine()
{
#ifdef _DEBUG
	constexpr bool bDebug = true;
#else
	constexpr bool bDebug = false;
#endif

	_engine = CreateSubobject<GameEngine>(bDebug);
	_engine->InitEngine(this);

	_world = CreateSubobject<World>();
	_world->LoadLevel(StartupLevel);
}