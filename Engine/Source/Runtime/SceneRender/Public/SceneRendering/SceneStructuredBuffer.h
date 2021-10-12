// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIResource;

struct SceneStructuredBuffer
{
	void* pSysMem;
	size_t SlicePitch;
	uint64 GPUVirtualAddressOffset;
};