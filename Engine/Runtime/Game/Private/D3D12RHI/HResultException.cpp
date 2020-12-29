// Copyright 2020 Aumoa.lib. All right reserved.

#include "HResultException.h"

#include <comdef.h>
#include "D3D12Minimal.h"
#include "Logging/LogMacros.h"

HResultException::HResultException(HRESULT hr) : Super(GetHResultString(hr))
{

}

HResultException::~HResultException()
{

}

TRefPtr<String> HResultException::GetHResultString(HRESULT hr)
{
	_com_error error(hr);
	
	TRefPtr<String> errorMsg = error.ErrorMessage();
	SE_LOG(LogD3D12RHI, Error, L"HRESULT exception(0x{0:X8}) occurred with message: {1}", (int32)hr, errorMsg);
	return String::Format(L"0x{0:X8}: {1}", (int32)hr, errorMsg);
}