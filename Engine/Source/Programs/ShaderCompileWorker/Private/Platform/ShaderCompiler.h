// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NShaderCompiler
{
private:
	static std::vector<DirectoryReference> IncludeDirectories;

public:
	virtual Task<> CompileVertexShaderAsync(String InName, String ShaderCode, const CancellationToken& cancellationToken) = 0;
	virtual Task<> CompilePixelShaderAsync(String InName, String ShaderCode, const CancellationToken& cancellationToken) = 0;

	virtual std::span<const byte> GetCompileResults() const = 0;
	virtual std::span<const FileReference> GetCompilerIncludes() const = 0;

public:
	static std::shared_ptr<NShaderCompiler> GeneratePlatformCompiler();

	static void SetIncludeDirectory(std::vector<DirectoryReference> InIncludeDirectories);
	inline static std::span<const DirectoryReference> GetIncludeDirectory() { return IncludeDirectories; }
};