// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "DXCCommon.h"

namespace Ayla
{
	class ShaderCompilationTask;

	class DXCIncludeHandler : public IDxcIncludeHandler
	{
		ComPtr<IDxcUtils> m_Utils;
		std::atomic<uint64> m_Refs = 1;  // Start with 1 ref count
		std::vector<std::pair<String, ComPtr<IDxcBlob>>> m_IncludeBlobs;
		std::vector<String> m_IncludeDirectories;

	public:
		DXCIncludeHandler(ComPtr<IDxcUtils> utils, const ShaderCompilationTask& task);

		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override;
		virtual ULONG STDMETHODCALLTYPE AddRef() override;
		virtual ULONG STDMETHODCALLTYPE Release() override;

		virtual HRESULT STDMETHODCALLTYPE LoadSource(LPCWSTR pFilename, IDxcBlob** ppIncludeSource) override;

		std::vector<String> GetDependentFiles() const;
	};
}