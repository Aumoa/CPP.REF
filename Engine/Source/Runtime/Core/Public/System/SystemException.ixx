// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:SystemException;

export import :Exception;

export class CORE_API SystemException : public Exception
{
public:
	SystemException(int32 InSystemCode) noexcept;
	SystemException(const std::error_code& InErrorCode) noexcept : SystemException(InErrorCode.value()) {}
	SystemException(int32 InSystemCode, String InMessage) noexcept;
	SystemException(const std::error_code& InErrorCode, String InMessage) noexcept : SystemException(InErrorCode.value(), InMessage) {}

private:
	static String FormatMessage(int32 InSystemCode, String InMessage = TEXT("")) noexcept;
};