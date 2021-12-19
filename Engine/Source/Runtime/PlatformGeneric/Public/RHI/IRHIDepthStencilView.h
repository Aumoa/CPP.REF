// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIView.h"
#include "RHIStructures.h"

interface IRHIDepthStencilView : implements IRHIView
{
	GENERATED_INTERFACE_BODY(IRHIDepthStencilView)

	virtual void CreateDepthStencilView(int32 index, IRHIResource* resource, const RHIDepthStencilViewDesc* desc) = 0;
};