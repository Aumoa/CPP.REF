// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/ShaderCompilerException.h"

using namespace ::libty;

ShaderCompilerException::ShaderCompilerException(std::string_view API, std::string_view message, std::source_location location)
	: FatalException(message, nullptr, location)
	, _api(API)
{
}