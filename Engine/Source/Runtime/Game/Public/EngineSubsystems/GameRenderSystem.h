// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"
#include "RenderThread.h"
#include "RHI/RHIInterfaces.h"

class SSlateApplication;
class SColorShaderWorkspace;

class GAME_API SGameRenderSystem : public SGameEngineSubsystem
{
	GENERATED_BODY(SGameRenderSystem)

private:
	std::optional<::RenderThread> RenderThread;

	SPROPERTY(Factory)
	IRHIFactory* Factory = nullptr;
	SPROPERTY(Device)
	IRHIDevice* Device = nullptr;
	SPROPERTY(PrimaryQueue)
	IRHICommandQueue* PrimaryQueue = nullptr;

	SPROPERTY(SwapChain)
	IRHISwapChain* SwapChain = nullptr;
	Vector2N SwapChainSize;

	SPROPERTY(CommandFence)
	IRHIFence* CommandFence = nullptr;
	uint64 _fenceValue = 0;

	SPROPERTY(RTV)
	IRHIRenderTargetView* RTV = nullptr;

	SPROPERTY(ShaderCodeWorkspace)
	SColorShaderWorkspace* ShaderCodeWorkspace = nullptr;

public:
	SGameRenderSystem();
	virtual ~SGameRenderSystem() override;

	virtual void Init() override;
	virtual void Deinit() override;
	virtual void ExecuteRenderThread(float InDeltaTime, SSlateApplication* SlateApp);

	IRHIDevice* GetRHIDevice();

private:
	void ResizeApp(Vector2N Size);
	void OnPreDestroyApp();

	void OnRender_RenderThread(IRHIGraphicsCommandList* Cmdbuf);
	void OnResizeApp_RenderThread(IRHIGraphicsCommandList* Cmdbuf, Vector2N Size);
};