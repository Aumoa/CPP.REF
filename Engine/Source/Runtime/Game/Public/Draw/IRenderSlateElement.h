// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "LocalRenderLayout.h"
#include "Layout/Layout.h"

interface IRHIDeviceContext2D;

interface IRenderSlateElement : implements SObject
{
	GENERATED_INTERFACE_BODY(IRenderSlateElement)
		
	virtual int32 GetLayer() = 0;
	virtual Geometry GetCachedGeometry() = 0;

	virtual void RenderElement(IRHIDeviceContext2D* CommandBuffer, const LocalRenderLayout& LocalLayout) = 0;
};