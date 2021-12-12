// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIView.h"
#include "RHIStructures.h"

interface IRHIShaderResourceView : implements IRHIView
{
	virtual void CreateShaderResourceView(int32 Index, IRHIResource* Resource, const RHIShaderResourceViewDesc* Desc) = 0;
};