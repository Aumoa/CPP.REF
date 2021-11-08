// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IApplicationInterface;
interface IRHIDeviceContext;
class SLocalPlayer;
class SWindow;
class SceneRenderContext;
class SlateWindowElementList;
class SlateRenderer;

class GAME_API SSlateApplication : implements SObject
{
	GENERATED_BODY(SSlateApplication)

private:
	SLocalPlayer* LocalPlayer = nullptr;
	SWindow* CoreWindow = nullptr;
	IRHIDeviceContext* DeviceContext = nullptr;

	std::shared_ptr<SlateWindowElementList> SlateElements;

public:
	SSlateApplication();
	~SSlateApplication() override;

	void Init(IApplicationInterface* InApplication);
	void TickAndPaint(float InDeltaTime);
	void DrawElements(SlateRenderer* Renderer);

	SLocalPlayer* GetLocalPlayer();
	SWindow* GetCoreWindow();

private:
	void OnPlatformKeyPressed(EKey InKey);
	void OnPlatformKeyReleased(EKey InKey);

	void OnCursorMoved(Vector2N Location);
	void OnMouseButtonPressed(Vector2N Location, EMouseButton Button);
	void OnMouseButtonReleased(Vector2N Location, EMouseButton Button);
	void OnMouseWheelScrolled(int32 ScrollDelta);
};