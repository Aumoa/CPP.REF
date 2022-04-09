// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline PlatformGeneric
{
	class PLATFORMGENERIC_API ShaderCompilerException : public FatalException
	{
		std::string _api;

	public:
		ShaderCompilerException(std::string_view API, std::string_view message, std::source_location location = std::source_location::current());
	};
}