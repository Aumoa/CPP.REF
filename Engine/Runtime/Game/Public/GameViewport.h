// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIResource;
interface IRHIRenderTargetView;
interface IRHICommandList;
interface IRHIDepthStencilView;

class Scene;

class GAME_API GameViewport : virtual public Object
{
	friend class Engine;

public:
	using Super = Object;
	using This = GameViewport;

private:
	int32 resX;
	int32 resY;
	TRefPtr<IRHIResource> renderTarget;
	TRefPtr<IRHIResource> depthStencilBuffer;
	TRefPtr<IRHIRenderTargetView> renderTargetView;
	TRefPtr<IRHIDepthStencilView> depthStencilView;

public:
	GameViewport();
	~GameViewport() override;

	virtual void RenderScene(IRHICommandList* inCommandList, Scene* inScene);

	IRHIResource* GetRenderTarget() const;
	IRHIResource* GetDepthStencilBuffer() const;

	vs_property_get(int32, ResolutionX);
	int32 ResolutionX_get() const;
	vs_property_get(int32, ResolutionY);
	int32 ResolutionY_get() const;

protected:
	virtual void SetViewportResolution_Internal(int32 x, int32 y);

private:
	void BeginRender(IRHICommandList* inCommandList);
	void EndRender(IRHICommandList* inCommandList);
};