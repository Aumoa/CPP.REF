// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "ShaderCompiler.h"
#include "Windows/HLSLCompiler.h"

std::vector<DirectoryReference> NShaderCompiler::IncludeDirectories;

std::shared_ptr<NShaderCompiler> NShaderCompiler::GeneratePlatformCompiler()
{
#if PLATFORM_WINDOWS
	return std::make_shared<NHLSLCompiler>();
#else
	throw NotSupportedException(TEXT("Platform is not supported."));
#endif
}

void NShaderCompiler::SetIncludeDirectory(std::vector<DirectoryReference> InIncludeDirectories)
{
	IncludeDirectories = InIncludeDirectories;
}