// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDeviceContext;

interface IRHIDevice : implements SObject
{
	virtual void Present() = 0;
	virtual void ResizeBuffers(const Vector2N& InPixelSize) = 0;

	virtual void BeginDraw(const Color& ClearColor) = 0;
	virtual void EndDraw() = 0;
};