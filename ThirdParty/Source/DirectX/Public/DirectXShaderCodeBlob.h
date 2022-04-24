// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "DirectXDeviceChild.h"

namespace libty::inline DirectX
{
	class DIRECTX_API SDirectXShaderCodeBlob : public SDirectXDeviceChild, implements(IRHIShaderCodeBlob)
	{
		GENERATED_BODY(SDirectXShaderCodeBlob);

	private:
		ComPtr<ID3DBlob> pBlob;
		ERHIShaderType ShaderType;

	public:
		SDirectXShaderCodeBlob(SDirectXDevice* Owner, ComPtr<ID3DBlob> pBlob, ERHIShaderType ShaderType);

		using Super::Dispose;

		virtual const void* GetBufferPointer() override;
		virtual size_t GetBufferSize() override;
		virtual ERHIShaderType GetShaderCodeType() override;

	protected:
		virtual void Dispose(bool bDisposing) override;
	};
}