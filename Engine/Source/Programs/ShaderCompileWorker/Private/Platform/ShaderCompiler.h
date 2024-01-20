// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NShaderCompiler
{
private:
	static DirectoryReference IncludeDirectory;

public:
	virtual Task<std::vector<byte>> CompileVertexShaderAsync(String InName, String ShaderCode, std::stop_token InCancellationToken) = 0;
	virtual Task<std::vector<byte>> CompilePixelShaderAsync(String InName, String ShaderCode, std::stop_token InCancellationToken) = 0;

public:
	static std::shared_ptr<NShaderCompiler> GeneratePlatformCompiler();

	static void SetIncludeDirectory(DirectoryReference InIncludeDirectory);
	inline static DirectoryReference GetIncludeDirectory() { return IncludeDirectory; }
};