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
	InitializeEngine();
	PostInitialized.Invoke();

	frameworkView->Start();
	return 0;
}

void GameInstance::InitializeEngine()
{
	_device = CreateSubobject<RHIDevice>();
}