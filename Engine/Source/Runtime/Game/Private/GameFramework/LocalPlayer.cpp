// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "GameFramework/LocalPlayer.h"
#include "GameEngine.h"
#include "GameInstance.h"
#include "IFrameworkView.h"
#include "Widgets/Window.h"
#include "Draw/SlateWindowElementList.h"
#include "Shaders/SlateShader/SlateShader.h"

#include "Assets/Texture2D.h"

LocalPlayer::LocalPlayer() : Super()
{
}

void LocalPlayer::Init(IFrameworkView* frameworkView)
{
	_frameworkView = frameworkView;

	_slateWindow = NewObject<SWindow>();

	// Register ResizeApp handler.
	_frameworkView->Size.AddObject(this, &LocalPlayer::OnResizedApp);
	// And apply size immediately.
	_slateWindow->SetWindowSize(Vector2((float)_frameworkView->GetFrameworkWidth(), (float)_frameworkView->GetFrameworkHeight()));

	_drawElements = NewObject<SlateWindowElementList>(_slateWindow);
}

void LocalPlayer::Tick(std::chrono::duration<float> elapsedTime)
{
	_slateWindow->ExecuteTick(elapsedTime);
}

void LocalPlayer::Render(RHIDeviceContext* deviceContext, SlateShader* shader)
{
	_drawElements->Clear();
	_slateWindow->ExecutePaint(_drawElements);

	auto* loadObject = LoadObject<Texture2D>(L"Content/THStory/Art/SampleImage.jpg");
	int32 width, height;
	loadObject->GetPixelSize(&width, &height);

	SlateBrush brush;
	brush.ImageSource = loadObject->GetShaderResourceView();
	brush.ImageSize = Vector2((float)width, (float)height);
	_drawElements->Add(SlateDrawElement(brush, PaintGeometry(Vector2(851.0f, 1087.0f), SlateRenderTransform::Identity()), 0));
	shader->RenderElements(deviceContext, _slateWindow->GetDesiredSize(), _drawElements);
}

void LocalPlayer::OnResizedApp(int32 x, int32 y)
{
	_slateWindow->SetWindowSize(Vector2((float)x, (float)y));
}