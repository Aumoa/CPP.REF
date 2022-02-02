// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/Layout.h"
#include "Threading/Task.h"
#include "Draw/SlateDrawCollector.h"
#include "Input/IMEEvent.h"

interface IRHIDevice;
interface IApplicationInterface;
interface IRHIDeviceContext;
interface IRHIDeviceContext2D;
interface IRenderSlateElement;
class SLocalPlayer;
class SWindow;
class SceneRenderContext;
class SSlateDrawCollector;
class SSlateRenderer;

class GAME_API SSlateApplication : implements SObject
{
	GENERATED_BODY(SSlateApplication)

private:
	SPROPERTY(Device)
	IRHIDevice* Device = nullptr;

	SPROPERTY(LocalPlayer)
	SLocalPlayer* LocalPlayer = nullptr;
	SPROPERTY(CoreWindow)
	SWindow* CoreWindow = nullptr;
	SPROPERTY(DeviceContext)
	IRHIDeviceContext* DeviceContext = nullptr;
	SPROPERTY(DrawCollector)
	SSlateDrawCollector* DrawCollector = nullptr;

	SPROPERTY(RenderElements)
	std::vector<SSlateDrawCollector::RenderElement> RenderElements;

public:
	SSlateApplication();
	~SSlateApplication() override;

	void Init(IApplicationInterface* InApplication);
	void TickAndPaint(float InDeltaTime);
	void DrawElements(IRHIDeviceContext2D* CommandList, SSlateRenderer* Renderer);

	SLocalPlayer* GetLocalPlayer();
	SWindow* GetCoreWindow();

protected:
	virtual SWindow* CreateCoreWindow();

private:
	void OnPlatformKeyPressed(EKey InKey);
	void OnPlatformKeyReleased(EKey InKey);

	void OnCursorMoved(Vector2N Location);
	void OnMouseButtonPressed(Vector2N Location, EMouseButton Button);
	void OnMouseButtonReleased(Vector2N Location, EMouseButton Button);
	void OnMouseWheelScrolled(int32 ScrollDelta);
	void OnIME(IMEEvent EventArgs);

	void CacheRenderElements_GameThread(std::vector<SSlateDrawCollector::RenderElement> Elements);
	Geometry MakeRoot();
};