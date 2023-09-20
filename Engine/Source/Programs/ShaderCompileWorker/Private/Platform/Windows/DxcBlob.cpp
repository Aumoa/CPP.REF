// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Platform/Windows/DxcBlob.h"

#if PLATFORM_WINDOWS

DxcBlob::DxcBlob(std::vector<byte> InBytes) : Bytes(std::move(InBytes)), Refs(1)
{
}

DxcBlob::DxcBlob(std::string InStrBytes) : Bytes(std::move(InStrBytes)), Refs(1)
{
}

HRESULT STDMETHODCALLTYPE DxcBlob::QueryInterface(REFIID riid, void** ppvObject)
{
	if (riid == __uuidof(IUnknown))
	{
		*ppvObject = (IUnknown*)this;
	}
	else if (riid == __uuidof(IDxcBlob))
	{
		*ppvObject = (IDxcBlob*)this;
	}
	else
	{
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG STDMETHODCALLTYPE DxcBlob::AddRef()
{
	return (ULONG)PlatformAtomics::InterlockedIncrement(&Refs);
}

ULONG STDMETHODCALLTYPE DxcBlob::Release()
{
	ULONG uRef = (ULONG)PlatformAtomics::InterlockedDecrement(&Refs);
	if (uRef == 0)
	{
		delete this;
	}
	return uRef;
}

LPVOID STDMETHODCALLTYPE DxcBlob::GetBufferPointer()
{
	size_t Idx = Bytes.index();
	if (Idx == 0)
	{
		return std::get<0>(Bytes).data();
	}
	else if (Idx == 1)
	{
		return std::get<1>(Bytes).data();
	}
	check(false);
	return nullptr;
}

SIZE_T STDMETHODCALLTYPE DxcBlob::GetBufferSize()
{
	size_t Idx = Bytes.index();
	if (Idx == 0)
	{
		return std::get<0>(Bytes).size();
	}
	else if (Idx == 1)
	{
		return std::get<1>(Bytes).length();
	}
	check(false);
	return 0;
}

#endif