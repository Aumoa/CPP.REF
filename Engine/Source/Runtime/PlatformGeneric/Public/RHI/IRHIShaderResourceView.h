// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIView.h"
#include "RHIStructures.h"

interface IRHIShaderResourceView : implements IRHIView
{
	GENERATED_INTERFACE_BODY(IRHIShaderResourceView)

	virtual void CreateShaderResourceView(int32 index, IRHIResource* resource, const RHIShaderResourceViewDesc* desc) = 0;
};