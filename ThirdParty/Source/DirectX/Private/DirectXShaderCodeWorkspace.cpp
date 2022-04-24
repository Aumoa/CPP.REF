// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXShaderCodeWorkspace.h"
#include "DirectXShaderCodeBlob.h"
#include "DirectXDevice.h"
#include <filesystem>

using namespace ::libty;

SDirectXShaderCodeWorkspace::SDirectXShaderCodeWorkspace(SDirectXDevice* Owner)
	: Super(Owner)
{
}

void SDirectXShaderCodeWorkspace::AddShaderCode(std::wstring_view Name, const RHIShaderCode& Code)
{
	auto Emplace_it = ShaderCodes.emplace(Name, Code);
	if (!Emplace_it.second)
	{
		throw ShaderCompilerException("DirectX", "ShaderCodeName duplicated.");
	}
}

void SDirectXShaderCodeWorkspace::Compile()
{
	for (auto& [Id, Code] : ShaderCodes)
	{
		if (!Code.EntryPoint.empty())
		{
			std::string AId = String::AsMultibyte(Id);
			std::string_view ATarget = ShaderTypeToTarget(Code.ShaderType);

			ComPtr<ID3DBlob> pBlob;
			ComPtr<ID3DBlob> pError;
			HRESULT HResult = D3DCompile(Code.HLSLCode.c_str(), Code.HLSLCode.length(), AId.c_str(), nullptr, this, Code.EntryPoint.c_str(), ATarget.data(), 0, 0, &pBlob, &pError);
			if (FAILED(HResult))
			{
				if (pError)
				{
					throw ShaderCompilerException("DirectX", std::format("Compile error occurred while compile '{}' shader code. ErrorTrace: \n{}", AId, std::string_view((const char*)pError->GetBufferPointer())));
				}
				else
				{
					ReportCOMError(L"D3DCompile error in DirectX API.", HResult);
				}
			}

			CompiledShaderBlobs.emplace(Code.EntryPoint, gcnew SDirectXShaderCodeBlob(Cast<SDirectXDevice>(GetDevice()), std::move(pBlob), Code.ShaderType));
		}
	}
}

IRHIShaderCodeBlob* SDirectXShaderCodeWorkspace::GetCompiledShaderCodeBlob(std::string_view EntryPointName)
{
	auto It = CompiledShaderBlobs.find(EntryPointName);
	if (It == CompiledShaderBlobs.end())
	{
		return nullptr;
	}

	return It->second;
}

HRESULT CALLBACK SDirectXShaderCodeWorkspace::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes)
{
	auto It = ShaderCodes.find(String::AsUnicode(pFileName));
	if (It == ShaderCodes.end())
	{
		return E_FAIL;
	}

	*pBytes = (UINT)It->second.HLSLCode.length();
	if (ppData)
	{
		*ppData = It->second.HLSLCode.c_str();
	}

	return S_OK;
}

HRESULT CALLBACK SDirectXShaderCodeWorkspace::Close(LPCVOID pData)
{
	return S_OK;
}

void SDirectXShaderCodeWorkspace::Dispose(bool bDisposing)
{
	ShaderCodes.clear();
	CompiledShaderBlobs.clear();

	Super::Dispose(bDisposing);
}