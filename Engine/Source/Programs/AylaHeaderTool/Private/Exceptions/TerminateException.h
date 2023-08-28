// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class TerminateException : public Exception
{
public:
	enum class EKnownErrorCodes
	{
		Success = 0,
		Usage = 1,
	};

public:
	TerminateException(EKnownErrorCodes ErrorCode);

	const EKnownErrorCodes ErrorCode;

private:
	static String FormatMessage(EKnownErrorCodes ErrorCode);
};