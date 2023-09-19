// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Exceptions/TerminateException.h"

TerminateException::TerminateException(EKnownErrorCodes ErrorCode)
	: Exception(FormatMessage(ErrorCode))
	, ErrorCode(ErrorCode)
{
}

String TerminateException::FormatMessage(EKnownErrorCodes ErrorCode)
{
	String Message;

	switch (ErrorCode)
	{
	case EKnownErrorCodes::Success:
		Message = TEXT("Working success.");
		break;
	case EKnownErrorCodes::NotSupportedShaderType:
		Message = TEXT("ShaderType is not supported.");
		break;
	default:
		Message = TEXT("Internal compiler error.");
		break;
	}

	return String::Format(TEXT("A{:0>4}: {}"), (int32)ErrorCode, Message);
}