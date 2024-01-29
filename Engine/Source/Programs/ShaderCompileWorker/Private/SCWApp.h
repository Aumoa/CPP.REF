// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NShaderCompiler;

class NSCWApp
{
public:
	NSCWApp();

	Task<> RunAsync(std::stop_token InCancellationToken);
	void PrintUsage(TextWriter& Writer);

private:
	static Task<> WriteHeaderOutputAsync(NShaderCompiler* Compiler, DirectoryReference OutputDirectory, String Name, String ShaderCode, std::stop_token InCancellationToken);
	static Task<> WriteDependencyCacheAsync(NShaderCompiler* Compiler, DirectoryReference OutputDirectory, String Name, std::stop_token InCancellationToken);
};