// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "ShaderCompiler.h"
#include "Windows/HLSLCompiler.h"

std::shared_ptr<NShaderCompiler> NShaderCompiler::GeneratePlatformCompiler()
{
#if PLATFORM_WINDOWS
	return std::make_shared<NHLSLCompiler>();
#else
	throw NotSupportedException(TEXT("Platform is not supported."));
#endif
}