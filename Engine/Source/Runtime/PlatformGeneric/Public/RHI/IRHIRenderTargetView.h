// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIView.h"
#include "RHIStructures.h"

interface IRHIRenderTargetView : implements IRHIView
{
	virtual void CreateRenderTargetView(int32 index, IRHIResource* resource, const RHIRenderTargetViewDesc* desc) = 0;
};