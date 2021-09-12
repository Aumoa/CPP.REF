// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "GameEngineSubsystem.h"

class RHIDevice;
class RHICommandQueue;
class RHISwapChain;
class RHIDeviceContext;
class RHIRenderTargetView;
class RHITexture2D;
class RHIDepthStencilView;
class ColorVertexFactory;
class ColorShader;
class AssetImporter;
class TransparentShader;
class SlateShader;
class TickScheduler;
class FreeTypeModule;
class FontCachingManager;
struct IFrameworkView;

class GAME_API GameRenderSystem : public GameEngineSubsystem
{
	GENERATED_BODY(GameRenderSystem)

private:
	IFrameworkView* _frameworkView = nullptr;
	TickScheduler* _scheduler = nullptr;

	RHIDevice* _device = nullptr;
	RHICommandQueue* _primaryQueue = nullptr;
	RHISwapChain* _frameworkViewChain = nullptr;
	RHIDeviceContext* _deviceContext = nullptr;
	ColorVertexFactory* _colorVertexFactory = nullptr;
	ColorShader* _colorShader = nullptr;
	RHIRenderTargetView* _rtv = nullptr;
	AssetImporter* _assimp = nullptr;
	RHITexture2D* _depthBuffer = nullptr;
	RHIDepthStencilView* _dsv = nullptr;
	TransparentShader* _transparentShader = nullptr;
	SlateShader* _slateShader = nullptr;
	FreeTypeModule* _freeTypeModule = nullptr;
	FontCachingManager* _fontCachingManager = nullptr;

	int32 _vpWidth = 0;
	int32 _vpHeight = 0;

public:
	GameRenderSystem();
	virtual ~GameRenderSystem() override;

	virtual void Init() override;
	virtual void Present();

	void SetupFrameworkView(IFrameworkView* frameworkView);
	IFrameworkView* GetFrameworkView() const;
	inline RHIDevice* GetRHIDevice() const { return _device; }

private:
	void Collect();
	void ResizeApp(int32 width, int32 height);
};