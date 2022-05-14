// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline PlatformGeneric
{
	class PLATFORMGENERIC_API ShaderCompilerException : public Exception
	{
		String _api;

	public:
		ShaderCompilerException(String API, String message);
	};
}