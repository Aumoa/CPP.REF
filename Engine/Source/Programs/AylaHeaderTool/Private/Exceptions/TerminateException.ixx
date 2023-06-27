// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module AylaHeaderTool:TerminateException;

export import Core;

export class TerminateException : public Exception
{
public:
	enum class EKnownErrorCodes
	{
		Success = 0,
	};

public:
	TerminateException(EKnownErrorCodes ErrorCode);

private:
	static String FormatMessage(EKnownErrorCodes ErrorCode);
};