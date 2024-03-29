// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Platform/Windows/DxcIncludeHandler.h"

#if PLATFORM_WINDOWS

#include "Platform/Windows/DxcBlob.h"

DxcIncludeHandler::DxcIncludeHandler(std::span<const DirectoryReference> InIncludeDirectories)
	: IncludeDirectories(InIncludeDirectories)
	, Refs(1)
{
}

HRESULT STDMETHODCALLTYPE DxcIncludeHandler::QueryInterface(REFIID riid, void** ppvObject)
{
	if (riid == __uuidof(IUnknown))
	{
		*ppvObject = (IUnknown*)this;
	}
	else if (riid == __uuidof(IDxcIncludeHandler))
	{
		*ppvObject = (IDxcIncludeHandler*)this;
	}
	else
	{
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG STDMETHODCALLTYPE DxcIncludeHandler::AddRef()
{
	return (ULONG)PlatformAtomics::InterlockedIncrement(&Refs);
}

ULONG STDMETHODCALLTYPE DxcIncludeHandler::Release()
{
	ULONG uRef = (ULONG)PlatformAtomics::InterlockedDecrement(&Refs);
	if (uRef == 0)
	{
		delete this;
	}
	return uRef;
}

HRESULT STDMETHODCALLTYPE DxcIncludeHandler::LoadSource(LPCWSTR pFilename, IDxcBlob** ppIncludeSource)
{
	FileReference Filename = {String::FromLiteral(pFilename)};
	DirectoryReference SourceDirectory = Filename.GetDirectory();
	
	for (auto& IncludeDirectory : IncludeDirectories | Linq::Append(SourceDirectory))
	{
		FileReference Fr = IncludeDirectory.GetFile(Filename);
		if (Fr.IsExists())
		{
			std::string AnsiCode = Fr.ReadAllText().AsCodepage();
			*ppIncludeSource = new DxcBlob(std::move(AnsiCode));
			HandledIncludeFiles.emplace_back(Fr);
			return S_OK;
		}
	}

	return E_FAIL;
}

#endif