// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXShaderCodeBlob.h"

GENERATE_BODY(SDirectXShaderCodeBlob);

SDirectXShaderCodeBlob::SDirectXShaderCodeBlob(SDirectXDevice* Owner, ComPtr<ID3DBlob> pBlob, ERHIShaderType ShaderType)
	: Super(Owner)
	, pBlob(std::move(pBlob))
	, ShaderType(ShaderType)
{
}

const void* SDirectXShaderCodeBlob::GetBufferPointer()
{
	return pBlob->GetBufferPointer();
}

size_t SDirectXShaderCodeBlob::GetBufferSize()
{
	return pBlob->GetBufferSize();
}

ERHIShaderType SDirectXShaderCodeBlob::GetShaderCodeType()
{
	return ShaderType;
}

void SDirectXShaderCodeBlob::Dispose(bool bDisposing)
{
	Super::Dispose(bDisposing);
	pBlob.Reset();
}