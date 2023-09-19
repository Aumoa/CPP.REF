// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NShaderCompiler
{
public:
	virtual Task<std::vector<byte>> CompileVertexShaderAsync(String InShaderFile, std::stop_token InCancellationToken) = 0;
	virtual Task<std::vector<byte>> CompilePixelShaderAsync(String InShaderFile, std::stop_token InCancellationToken) = 0;

public:
	static std::shared_ptr<NShaderCompiler> GeneratePlatformCompiler();
};