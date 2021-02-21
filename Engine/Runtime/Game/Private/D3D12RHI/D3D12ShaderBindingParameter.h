// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "D3D12Minimal.h"

struct D3D12ShaderBindingParameter
{
	char ShaderIdentifier[D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES];
	D3D12_GPU_DESCRIPTOR_HANDLE RootParameters[0];
};