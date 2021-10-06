// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameFramework/LocalPlayer.h"
#include "GameEngine.h"
#include "GameInstance.h"
#include "IFrameworkView.h"
#include "Widgets/SlateApplication.h"
#include "Draw/SlateWindowElementList.h"
#include "Shaders/SlateShader/SlateShader.h"

SLocalPlayer::SLocalPlayer() : Super()
{
}

void SLocalPlayer::Init(IFrameworkView* frameworkView)
{
	_frameworkView = frameworkView;

	_slateWindow = SNew(SSlateApplication);
	_slateWindow->Init();

	// Register ResizeApp handler.
	_frameworkView->Size.AddSObject(this, &SLocalPlayer::OnResizedApp);
	// And apply size immediately.
	_slateWindow->SetWindowSize(_frameworkView->GetFrameworkSize().Cast<float>());

	_drawElements = NewObject<SSlateWindowElementList>(_slateWindow);
}

void SLocalPlayer::Tick(std::chrono::duration<float> elapsedTime)
{
	_slateWindow->ExecuteTick(elapsedTime);
}

void SLocalPlayer::Render(IRHIDeviceContext* deviceContext, SSlateShader* shader)
{
	_drawElements->Clear();
	_slateWindow->ExecutePaint(_drawElements);
	shader->RenderElements(deviceContext, _slateWindow->GetDesiredSize(), _drawElements);
}

void SLocalPlayer::OnResizedApp(int32 x, int32 y)
{
	_slateWindow->SetWindowSize(Vector2((float)x, (float)y));
}