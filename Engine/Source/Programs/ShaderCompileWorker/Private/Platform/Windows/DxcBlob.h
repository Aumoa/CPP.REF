// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__
#include "Platform/PlatformCommon.h"

class DxcBlob : public IDxcBlob
{
private:
	std::variant<std::vector<byte>, std::string> Bytes;
	size_t Refs;

public:
	DxcBlob(std::vector<byte> InBytes);
	DxcBlob(std::string InStrBytes);

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override;
	virtual ULONG STDMETHODCALLTYPE AddRef() override;
	virtual ULONG STDMETHODCALLTYPE Release() override;

	virtual LPVOID STDMETHODCALLTYPE GetBufferPointer() override;
	virtual SIZE_T STDMETHODCALLTYPE GetBufferSize() override;
};

#undef __ALLOW_PLATFORM_COMMON_H__
#endif