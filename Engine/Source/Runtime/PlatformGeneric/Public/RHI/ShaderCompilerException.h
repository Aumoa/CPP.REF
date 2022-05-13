// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline PlatformGeneric
{
	class PLATFORMGENERIC_API ShaderCompilerException : public Exception
	{
		std::string _api;

	public:
		ShaderCompilerException(std::string_view API, std::string_view message);
	};
}