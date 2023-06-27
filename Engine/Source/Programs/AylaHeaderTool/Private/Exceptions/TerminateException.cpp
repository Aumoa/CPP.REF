// Copyright 2020-2022 Aumoa.lib. All right reserved.

import Core;
import AylaHeaderTool;
import <format>;

TerminateException::TerminateException(EKnownErrorCodes ErrorCode)
	: Exception(FormatMessage(ErrorCode))
{
}

String TerminateException::FormatMessage(EKnownErrorCodes ErrorCode)
{
	switch (ErrorCode)
	{
	case EKnownErrorCodes::Success:
		return TEXT("A0000: Working success.");
	default:
		return String::Format(TEXT("A{:0>4}: Unknown error code."), (int32)ErrorCode);
	}
}