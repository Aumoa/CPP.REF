// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Platform/ShaderCompiler.h"

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__
#include "Platform/PlatformCommon.h"

class NHLSLCompiler : public NShaderCompiler
{
	ComPtr<IDxcLibrary> pLibrary;
	ComPtr<IDxcCompiler> pCompiler;

public:
	NHLSLCompiler();

	virtual Task<std::vector<byte>> CompileVertexShaderAsync(String InShaderFile, std::stop_token InCancellationToken) override;
	virtual Task<std::vector<byte>> CompilePixelShaderAsync(String InShaderFile, std::stop_token InCancellationToken) override;

private:
	Task<std::vector<byte>> CompileShaderAsync(String InShaderFile, String InModelName, std::stop_token InCancellationToken);
};

#endif