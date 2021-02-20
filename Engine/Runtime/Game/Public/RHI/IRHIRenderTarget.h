﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHICommandList;
interface IRHIShaderResourceView;
interface IRHIResource;

interface GAME_API IRHIRenderTarget : virtual public Object
{
	virtual void ResizeBuffers(int32 width, int32 height) = 0;
	virtual size_t GetRenderTargetCount() const = 0;
	virtual IRHIResource* GetRenderTarget(size_t index) const = 0;
	virtual IRHIShaderResourceView* GetShaderResourceView() const = 0;
};