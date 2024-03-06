// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NShaderCompiler;

class NSCWApp
{
public:
	NSCWApp();

	Task<> RunAsync(const CancellationToken& cancellationToken);
	void PrintUsage(TextWriter& Writer);

private:
	static Task<> WriteHeaderOutputAsync(NShaderCompiler* Compiler, DirectoryReference OutputDirectory, String Name, String ShaderCode, const CancellationToken& cancellationToken);
	static Task<> WriteDependencyCacheAsync(NShaderCompiler* Compiler, DirectoryReference OutputDirectory, String Name, const CancellationToken& cancellationToken);
};