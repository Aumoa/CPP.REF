// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IApplicationInterface;
interface IRHIDeviceContext;
class SLocalPlayer;
class SWindow;
class SceneRenderContext;

class GAME_API SSlateApplication : implements SObject
{
	GENERATED_BODY(SSlateApplication)

private:
	SLocalPlayer* LocalPlayer = nullptr;
	SWindow* CoreWindow = nullptr;
	IRHIDeviceContext* DeviceContext = nullptr;

public:
	SSlateApplication();
	~SSlateApplication() override;

	void Init(IApplicationInterface* InApplication);
	void TickAndPaint(float InDeltaTime);
	void PopulateCommandLists(SceneRenderContext& RenderContext);

	SLocalPlayer* GetLocalPlayer();
};