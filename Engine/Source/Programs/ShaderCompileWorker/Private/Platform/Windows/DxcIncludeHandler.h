// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__
#include "Platform/PlatformCommon.h"

class DxcIncludeHandler : public IDxcIncludeHandler
{
private:
	std::span<const DirectoryReference> IncludeDirectories;
	size_t Refs;
	std::vector<FileReference> HandledIncludeFiles;

public:
	DxcIncludeHandler(std::span<const DirectoryReference> InIncludeDirectories);

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override;
	virtual ULONG STDMETHODCALLTYPE AddRef() override;
	virtual ULONG STDMETHODCALLTYPE Release() override;

	virtual HRESULT STDMETHODCALLTYPE LoadSource(LPCWSTR pFilename, IDxcBlob** ppIncludeSource) override;

	std::span<const FileReference> GetHandledIncludeFiles() const { return HandledIncludeFiles; }
};

#undef __ALLOW_PLATFORM_COMMON_H__
#endif