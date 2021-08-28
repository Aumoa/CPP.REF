// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIDeviceChild.h"

class RHIShaderDescriptorView;

/// <summary>
/// Represents resource view for binding to gpu context.
/// </summary>
class RENDERCORE_API RHIView : public RHIDeviceChild
{
	CLASS_BODY(RHIView)

public:
	RHIView(RHIDevice* device);
};