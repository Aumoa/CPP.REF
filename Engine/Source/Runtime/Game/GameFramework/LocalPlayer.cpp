// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "LocalPlayer.h"
#include "GameEngine.h"
#include "GameInstance.h"
#include "IFrameworkView.h"
#include "Widgets/Window.h"

using namespace std;
using namespace std::chrono;

LocalPlayer::LocalPlayer() : Super()
{
}

void LocalPlayer::Init(GameEngine* engine)
{
	_slateWindow = CreateSubobject<SWindow>();

	GameInstance* gameInstance = engine->GetGameInstance();
	_frameworkView = gameInstance->GetFrameworkView();
	check(_frameworkView);

	// Register ResizeApp handler.
	_frameworkView->Size.AddObject(this, &LocalPlayer::OnResizedApp);
	// And apply size immediately.
	_slateWindow->SetWindowSize(Vector2((float)_frameworkView->GetFrameworkWidth(), (float)_frameworkView->GetFrameworkHeight()));
}

void LocalPlayer::Tick(duration<float> elapsedTime)
{
	_slateWindow->ExecuteTick(elapsedTime);
}

void LocalPlayer::Render()
{
	SlateWindowElementList drawElements(_slateWindow);
	_slateWindow->ExecutePaint(&drawElements);
}

void LocalPlayer::OnResizedApp(int32 x, int32 y)
{
	_slateWindow->SetWindowSize(Vector2((float)x, (float)y));
}