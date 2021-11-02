// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneRenderTarget.h"

interface IRHIFactory;
interface IRHIDevice;
interface IRHISwapChain;

class GAME_API SSwapChainRenderTarget : implements SObject, public SceneRenderTarget
{
	GENERATED_BODY(SSwapChainRenderTarget)

public:
	static constexpr int32 NumBuffers = 3;

private:
	IRHISwapChain* SwapChain = nullptr;
	IRHITexture2D* Buffers[NumBuffers] = {};
	int32 PixelWidth;
	int32 PixelHeight;

public:
	SSwapChainRenderTarget(IRHIFactory* InFactory, IRHIDevice* InDevice);

	void ResizeBuffers(int32 InWidth, int32 InHeight);
	void ResolveTarget();
	void Present(bool bVSync = false);

private:
	void ReadyBuffers(int32 InWidth, int32 InHeight);
};