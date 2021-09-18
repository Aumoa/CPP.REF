// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "GameEngineSubsystem.h"

class SRHIDevice;
class SRHICommandQueue;
class SRHISwapChain;
class SRHIDeviceContext;
class SRHIRenderTargetView;
class SRHITexture2D;
class SRHIDepthStencilView;
class SColorVertexFactory;
class SColorShader;
class SAssetImporter;
class STransparentShader;
class SSlateShader;
class STickScheduler;
class SFreeTypeModule;
class SFontCachingManager;
struct IFrameworkView;

class GAME_API SGameRenderSystem : public SGameEngineSubsystem
{
	GENERATED_BODY(SGameRenderSystem)

private:
	IFrameworkView* _frameworkView = nullptr;
	STickScheduler* _scheduler = nullptr;

	SRHIDevice* _device = nullptr;
	SRHICommandQueue* _primaryQueue = nullptr;
	SRHISwapChain* _frameworkViewChain = nullptr;
	SRHIDeviceContext* _deviceContext = nullptr;
	SColorVertexFactory* _colorVertexFactory = nullptr;
	SColorShader* _colorShader = nullptr;
	SRHIRenderTargetView* _rtv = nullptr;
	SAssetImporter* _assimp = nullptr;
	SRHITexture2D* _depthBuffer = nullptr;
	SRHIDepthStencilView* _dsv = nullptr;
	STransparentShader* _transparentShader = nullptr;
	SSlateShader* _slateShader = nullptr;
	SFreeTypeModule* _freeTypeModule = nullptr;
	SFontCachingManager* _fontCachingManager = nullptr;

	int32 _vpWidth = 0;
	int32 _vpHeight = 0;

public:
	SGameRenderSystem();
	virtual ~SGameRenderSystem() override;

	virtual void Init() override;
	virtual void Present();

	void SetupFrameworkView(IFrameworkView* frameworkView);
	IFrameworkView* GetFrameworkView() const;
	inline SRHIDevice* GetRHIDevice() const { return _device; }

	SFreeTypeModule* GetFreeTypeModule() const { return _freeTypeModule; }
	SFontCachingManager* GetFontCachingManager() const { return _fontCachingManager; }

private:
	void Collect();
	void ResizeApp(int32 width, int32 height);
};