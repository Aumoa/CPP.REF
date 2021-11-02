// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"

interface IRHIFactory;
interface IRHIDevice;
interface IRHIDeviceContext;
interface IApplicationInterface;
class SColorVertexFactory;
class SColorShader;
class STransparentShader;
class SSlateShader;
class SSlateApplication;
class SSwapChainRenderTarget;

class GAME_API SGameRenderSystem : public SGameEngineSubsystem
{
	GENERATED_BODY(SGameRenderSystem)

private:
	IRHIFactory* Factory = nullptr;
	IRHIDevice* Device = nullptr;
	IRHIDeviceContext* PrimaryQueue = nullptr;
	SColorVertexFactory* ColorVertexFactory = nullptr;
	SColorShader* ColorShader = nullptr;
	STransparentShader* TransparentShader = nullptr;
	SSlateShader* SlateShader = nullptr;

	SSwapChainRenderTarget* SwapChainRT = nullptr;

public:
	SGameRenderSystem();
	virtual ~SGameRenderSystem() override;

	virtual void Init() override;
	virtual void Deinit() override;
	virtual void Tick(float InDeltaTime) override;
	virtual void ExecuteRenderThread(float InDeltaTime, SSlateApplication* SlateApp);

	IRHIDevice* GetRHIDevice();

	// Test feature.
	SColorVertexFactory* GetColorVertexFactory() { return ColorVertexFactory; }
	SSlateShader* GetSlateShader() { return SlateShader; }

private:
	void ResizeApp(Vector2N Size);
};