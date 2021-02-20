﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIResource;
interface IRHICommandList;

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

public:
	GameViewport();
	~GameViewport() override;

	virtual void RenderScene(IRHICommandList* inCommandList, Scene* inScene) = 0;
	virtual IRHIResource* GetRenderTarget() const = 0;

	vs_property_get(int32, ResolutionX);
	int32 ResolutionX_get() const;
	vs_property_get(int32, ResolutionY);
	int32 ResolutionY_get() const;

protected:
	virtual void SetViewportResolution_Internal(int32 x, int32 y);
};