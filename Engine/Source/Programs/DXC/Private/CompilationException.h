// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

namespace Ayla
{
	class CompilationException : public Exception
	{
	public:
		CompilationException(const String& InMessage)
			: Exception(String::Format(TEXT("Shader compilation error: {}"), InMessage))
		{
		}

		CompilationException(const String& InMessage, const String& InShaderFile)
			: Exception(String::Format(TEXT("Shader compilation error in '{}': {}"), InShaderFile, InMessage))
		{
		}
	};
}
