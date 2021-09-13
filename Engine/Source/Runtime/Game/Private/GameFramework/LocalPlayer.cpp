// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "GameFramework/LocalPlayer.h"
#include "GameEngine.h"
#include "GameInstance.h"
#include "IFrameworkView.h"
#include "Widgets/Window.h"
#include "Draw/SlateWindowElementList.h"
#include "Shaders/SlateShader/SlateShader.h"

SLocalPlayer::SLocalPlayer() : Super()
{
}

void SLocalPlayer::Init(IFrameworkView* frameworkView)
{
	_frameworkView = frameworkView;

	_slateWindow = SNew(SWindow);

	// Register ResizeApp handler.
	_frameworkView->Size.AddObject(this, &SLocalPlayer::OnResizedApp);
	// And apply size immediately.
	_slateWindow->SetWindowSize(Vector2((float)_frameworkView->GetFrameworkWidth(), (float)_frameworkView->GetFrameworkHeight()));

	_drawElements = NewObject<SSlateWindowElementList>(_slateWindow);
}

void SLocalPlayer::Tick(std::chrono::duration<float> elapsedTime)
{
	_slateWindow->ExecuteTick(elapsedTime);
}

void SLocalPlayer::Render(SRHIDeviceContext* deviceContext, SSlateShader* shader)
{
	_drawElements->Clear();
	_slateWindow->ExecutePaint(_drawElements);
	shader->RenderElements(deviceContext, _slateWindow->GetDesiredSize(), _drawElements);
}

void SLocalPlayer::OnResizedApp(int32 x, int32 y)
{
	_slateWindow->SetWindowSize(Vector2((float)x, (float)y));
}