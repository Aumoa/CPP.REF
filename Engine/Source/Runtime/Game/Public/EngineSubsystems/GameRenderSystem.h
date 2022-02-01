// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"
#include "RenderThread.h"

interface IRHIFactory;
interface IRHIDevice;
interface IRHIDeviceContext;
interface IApplicationInterface;
interface IRHIDeviceContext2D;
interface IRHITextFormat;
interface IRHITextLayout;
class SColorVertexFactory;
class SColorShader;
class STransparentShader;
class SSlateShader;
class SSlateApplication;
class SSwapChainRenderTarget;
class SColorRenderTarget;
class SSlateRenderer;

// TEST
interface IRHISolidColorBrush;

class GAME_API SGameRenderSystem : public SGameEngineSubsystem
{
	GENERATED_BODY(SGameRenderSystem)

private:
	std::optional<RenderThread> RenderThread;

	SPROPERTY(Factory)
	IRHIFactory* Factory = nullptr;
	SPROPERTY(Device)
	IRHIDevice* Device = nullptr;
	SPROPERTY(PrimaryQueue)
	IRHIDeviceContext* PrimaryQueue = nullptr;
	//SPROPERTY(ColorVertexFactory)
	//SColorVertexFactory* ColorVertexFactory = nullptr;
	//SPROPERTY(ColorShader)
	//SColorShader* ColorShader = nullptr;
	//SPROPERTY(TransparentShader)
	//STransparentShader* TransparentShader = nullptr;
	//SPROPERTY(SlateShader)
	//SSlateShader* SlateShader = nullptr;

	SPROPERTY(SwapChainRT)
	SSwapChainRenderTarget* SwapChainRT = nullptr;
	SPROPERTY(RenderContext)
	IRHIDeviceContext* RenderContext = nullptr;
	SPROPERTY(DeviceContext2D)
	IRHIDeviceContext2D* DeviceContext2D = nullptr;
	SPROPERTY(ColorRenderTarget)
	SColorRenderTarget* ColorRenderTarget = nullptr;

	SPROPERTY(SlateRenderer)
	SSlateRenderer* SlateRenderer = nullptr;

public:
	SGameRenderSystem();
	virtual ~SGameRenderSystem() override;

	virtual void Init() override;
	virtual void Deinit() override;
	virtual void Tick(float InDeltaTime) override;
	virtual void ExecuteRenderThread(float InDeltaTime, SSlateApplication* SlateApp);

	IRHIDevice* GetRHIDevice();

	// Test feature.
	//SColorVertexFactory* GetColorVertexFactory() { return ColorVertexFactory; }
	//SSlateShader* GetSlateShader() { return SlateShader; }

private:
	void ResizeApp(Vector2N Size);
};