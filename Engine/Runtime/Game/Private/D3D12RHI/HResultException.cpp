// Copyright 2020 Aumoa.lib. All right reserved.

#include "HResultException.h"

#include <comdef.h>

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game::D3D12RHI;

HResultException::HResultException(HRESULT hr) : Super(GetHResultString(hr))
{

}

HResultException::~HResultException()
{

}

TRefPtr<String> HResultException::GetHResultString(HRESULT hr)
{
	_com_error error(hr);
	return String::Format(L"0x{0:X8}: {1}", (int32)hr, error.ErrorMessage());
}