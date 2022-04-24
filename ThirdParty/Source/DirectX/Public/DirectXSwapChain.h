// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "DirectXDeviceChild.h"

namespace libty::inline DirectX
{
	class SDirectXDevice;
	class SDirectXResource;

	class DIRECTX_API SDirectXSwapChain : public SDirectXDeviceChild, implements(IRHISwapChain)
	{
		GENERATED_BODY(SDirectXSwapChain);

	public:
		ComPtr<IDXGISwapChain4> pSwapChain;
		SPROPERTY(Buffers)
		std::vector<SDirectXResource*> Buffers;

	public:
		SDirectXSwapChain(SDirectXDevice* Owner, ComPtr<IDXGISwapChain4> pSwapChain, size_t BufferCount);

		using Super::Dispose;

		virtual void ResizeBuffers(const Vector2N& size) override;
		virtual IRHIResource* GetBuffer(size_t index) override;
		virtual void Present() override;
		virtual size_t GetCurrentBackBufferIndex() override;

	protected:
		virtual void Dispose(bool bDisposing) override;
	};
}