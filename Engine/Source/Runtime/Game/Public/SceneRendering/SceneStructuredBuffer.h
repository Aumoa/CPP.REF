// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct IRHIResource;

struct SceneStructuredBuffer
{
	void* pSysMem;
	size_t SlicePitch;
	uint64 GPUVirtualAddressOffset;
};