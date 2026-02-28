// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "DXCIncludeHandler.h"
#include "HLSLSourceCollection.h"
#include "ShaderCompilationTask.h"
#include "IO/File.h"

namespace Ayla
{
	DXCIncludeHandler::DXCIncludeHandler(ComPtr<IDxcUtils> utils, const ShaderCompilationTask& task)
		: m_Utils(std::move(utils))
		, m_IncludeDirectories(task.GetIncludePaths())
	{
	}

	HRESULT STDMETHODCALLTYPE DXCIncludeHandler::QueryInterface(REFIID riid, void** ppvObject)
	{
		if (riid == IID_IUnknown || riid == IID_IDxcIncludeHandler)
		{
			*ppvObject = static_cast<IDxcIncludeHandler*>(this);
			AddRef();
			return S_OK;
		}

		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}

	ULONG STDMETHODCALLTYPE DXCIncludeHandler::AddRef()
	{
		return (ULONG)++m_Refs;
	}

	ULONG STDMETHODCALLTYPE DXCIncludeHandler::Release()
	{
		uint64 refs = --m_Refs;
		if (refs == 0)
		{
			delete this;
		}

		return (ULONG)refs;
	}

	bool TryLoadSource(IDxcUtils* utils, String fileName, std::vector<std::pair<String, ComPtr<IDxcBlob>>>& container, IDxcBlob** ppIncludeSource)
	{
		if (File::Exists(fileName))
		{
			auto ptr = container.emplace_back(fileName, HLSLSourceCollection::LoadOrGetSource(fileName, utils)).second.Get();
			*ppIncludeSource = ptr;
			return true;
		}

		return false;
	}

	HRESULT STDMETHODCALLTYPE DXCIncludeHandler::LoadSource(LPCWSTR pFilename, IDxcBlob** ppIncludeSource)
	{
		try
		{
			String fn = String(pFilename);
			if (TryLoadSource(m_Utils.Get(), fn, m_IncludeBlobs, ppIncludeSource))
			{
				return S_OK;
			}

			for (auto& includeDirectory : m_IncludeDirectories)
			{
				auto ffn = Path::Combine(includeDirectory, fn);
				if (TryLoadSource(m_Utils.Get(), ffn, m_IncludeBlobs, ppIncludeSource))
				{
					return S_OK;
				}
			}

			// File not found in any include directory
			return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
		}
		catch (const HRException& e)
		{
			return e.HResult;
		}
	}

	std::vector<String> DXCIncludeHandler::GetDependentFiles() const
	{
		std::vector<String> result;
		for (const auto& pair : m_IncludeBlobs)
		{
			result.push_back(pair.first);
		}
		return result;
	}
}