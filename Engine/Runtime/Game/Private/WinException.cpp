// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "WinException.h"

#include "WindowsMinimal.h"

WinException::WinException() : This(GetLastError())
{

}

WinException::WinException(int32 lastError) : Super(GetErrorMessage(lastError))
{

}

WinException::~WinException()
{

}

TRefPtr<String> WinException::GetErrorMessage(int32 lastError)
{
	wchar_t* buffer = nullptr;
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, (DWORD)lastError, 0, (LPWSTR)&buffer, 0, nullptr);

	if (buffer == nullptr)
	{
		return "Unexpected error.";
	}

	TRefPtr<String> message = buffer;
	LocalFree(buffer);

	return message;
}