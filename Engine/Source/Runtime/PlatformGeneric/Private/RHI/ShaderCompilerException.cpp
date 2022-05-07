// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/ShaderCompilerException.h"

using namespace ::libty;

ShaderCompilerException::ShaderCompilerException(std::string_view API, std::string_view message)
	: FatalException(message)
	, _api(API)
{
}