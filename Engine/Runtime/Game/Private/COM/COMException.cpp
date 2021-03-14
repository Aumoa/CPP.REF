// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "COM/COMException.h"

#include "Logging/LogMacros.h"

#undef interface
#include <comdef.h>

COMException::COMException(int32 errorCode) : Super(GetHResultString(errorCode))
{

}

COMException::~COMException()
{

}

bool COMException::Succeeded(int32 errorCode)
{
	return SUCCEEDED(errorCode);
}

bool COMException::Failed(int32 errorCode)
{
	return FAILED(errorCode);
}

TRefPtr<String> COMException::GetHResultString(int32 hr)
{
	_com_error error(hr);

	TRefPtr<String> errorMsg = error.ErrorMessage();
	SE_LOG(LogCOM, Error, L"HRESULT exception(0x{0:X8}) occurred with message: {1}", (int32)hr, errorMsg);
	return String::Format(L"0x{0:X8}: {1}", (int32)hr, errorMsg);
}