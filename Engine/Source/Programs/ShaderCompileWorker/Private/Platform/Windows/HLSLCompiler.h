// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Platform/ShaderCompiler.h"

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__
#include "Platform/PlatformCommon.h"
#include "Platform/Windows/DxcIncludeHandler.h"

class NHLSLCompiler : public NShaderCompiler
{
	ComPtr<IDxcLibrary> pLibrary;
	ComPtr<IDxcCompiler> pCompiler;
	ComPtr<DxcIncludeHandler> pIncludeHandler;
	ComPtr<IDxcBlob> pCompiledResults;

public:
	NHLSLCompiler();

	virtual Task<> CompileVertexShaderAsync(String InName, String ShaderCode, const CancellationToken& cancellationToken) override;
	virtual Task<> CompilePixelShaderAsync(String InName, String ShaderCode, const CancellationToken& cancellationToken) override;

	virtual std::span<const byte> GetCompileResults() const override;
	virtual std::span<const FileReference> GetCompilerIncludes() const override;

private:
	Task<> CompileShaderAsync(String InName, String ShaderCode, String InModelName, const CancellationToken& cancellationToken);
};

#endif