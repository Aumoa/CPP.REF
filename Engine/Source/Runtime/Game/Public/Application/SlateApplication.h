// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Shaders/SlateShader/SlateShader.h"

interface IFrameworkView;
interface IRHIDeviceContext;
class PaintArgs;
class SLocalPlayer;
class SWindow;
class SceneRenderContext;
class SlateWindowElementList;

class GAME_API SSlateApplication : implements SObject
{
	GENERATED_BODY(SSlateApplication)

private:
	SLocalPlayer* LocalPlayer = nullptr;
	SWindow* CoreWindow = nullptr;
	IRHIDeviceContext* DeviceContext = nullptr;

	SSlateShader::InitElementContext InitContext_RenderThread;

public:
	SSlateApplication();
	~SSlateApplication() override;

	void InitWindow(SLocalPlayer* InLocalPlayer, IFrameworkView* InFrameworkView);
	void TickAndPaint(float InDeltaTime);
	void PopulateCommandLists(SceneRenderContext& RenderContext);
};