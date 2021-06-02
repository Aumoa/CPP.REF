// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Core;
import SC.Runtime.Game;

using namespace std;

GameInstance::GameInstance(wstring_view name) : Super(name)
{
}

int32 GameInstance::Run(IFrameworkView* frameworkView)
{
	_frameworkView = frameworkView;

	InitializeEngine();
	PostInitialized.Invoke();

	frameworkView->Start();
	return 0;
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
}