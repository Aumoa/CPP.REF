// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/ShaderCompilerException.h"

using namespace ::libty;

ShaderCompilerException::ShaderCompilerException(String API, String message)
	: Exception(message)
	, _api(API)
{
}