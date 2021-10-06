// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"

interface IRHIFactory;
interface IRHIDevice;
interface IRHIDeviceContext;
interface IRHISwapChain;
interface IRHIDeviceContext;
interface IRHIRenderTargetView;
interface IRHIDepthStencilView;
interface IRHITexture2D;
class SColorVertexFactory;
class SColorShader;
class SAssetImporter;
class STransparentShader;
class SSlateShader;
class STickScheduler;
struct IFrameworkView;

class GAME_API SGameRenderSystem : public SGameEngineSubsystem
{
	GENERATED_BODY(SGameRenderSystem)

private:
	IFrameworkView* _frameworkView = nullptr;

	IRHIFactory* _factory = nullptr;
	IRHIDevice* _device = nullptr;
	IRHIDeviceContext* _primaryQueue = nullptr;
	IRHISwapChain* _frameworkViewChain = nullptr;
	IRHIDeviceContext* _deviceContext = nullptr;
	SColorVertexFactory* _colorVertexFactory = nullptr;
	SColorShader* _colorShader = nullptr;
	IRHIRenderTargetView* _rtv = nullptr;
	SAssetImporter* _assimp = nullptr;
	IRHITexture2D* _depthBuffer = nullptr;
	IRHIDepthStencilView* _dsv = nullptr;
	STransparentShader* _transparentShader = nullptr;
	SSlateShader* _slateShader = nullptr;

	int32 _vpWidth = 0;
	int32 _vpHeight = 0;

public:
	SGameRenderSystem();
	virtual ~SGameRenderSystem() override;

	virtual void Init() override;
	virtual void Present();

	void SetupFrameworkView(IFrameworkView* frameworkView);
	IFrameworkView* GetFrameworkView();
	inline IRHIDevice* GetRHIDevice() { return _device; }

private:
	void ResizeApp(int32 width, int32 height);
};