// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class TerminateException : public Exception
{
public:
	enum class EKnownErrorCodes
	{
		Success = 0,
		NotSupportedShaderType = 1,
		CompilerError = 2,
		InputFileNotFound = 3
	};

public:
	TerminateException(EKnownErrorCodes ErrorCode);

	const EKnownErrorCodes ErrorCode;

private:
	static String FormatMessage(EKnownErrorCodes ErrorCode);
};