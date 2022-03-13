// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "DirectXDeviceChild.h"

class SDirectXShaderCodeBlob;

class DIRECTX_API SDirectXShaderCodeWorkspace : public SDirectXDeviceChild, implements IRHIShaderCodeWorkspace, implements ID3DInclude
{
	GENERATED_BODY(SDirectXShaderCodeWorkspace)

private:
	std::map<std::wstring, RHIShaderCode, std::less<>> ShaderCodes;
	SPROPERTY(CompiledShaderBlobs)
	std::map<std::wstring, SDirectXShaderCodeBlob*, std::less<>> CompiledShaderBlobs;

public:
	SDirectXShaderCodeWorkspace(SDirectXDevice* Owner);

	using Super::Dispose;

	virtual void AddShaderCode(std::wstring_view Name, RHIShaderCode Code) override;
	virtual void Compile() override;
	virtual IRHIShaderCodeBlob* GetCompiledShaderCodeBlob(std::wstring_view Name) override;

	virtual HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override;
	virtual HRESULT __stdcall Close(LPCVOID pData) override;

protected:
	virtual void Dispose(bool bDisposing) override;
};