// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIView.h"
#include "RHIStructures.h"

interface IRHIDepthStencilView : implements IRHIView
{
	virtual void CreateDepthStencilView(int32 Index, std::shared_ptr<IRHIResource> Resource, const RHIDepthStencilViewDesc* pDesc) = 0;
};