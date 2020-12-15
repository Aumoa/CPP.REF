// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHISwapChain.h"

#include "D3D12Minimal.h"

namespace SC::Runtime::Game::D3D12RHI
{
	class D3D12SwapChain : virtual public Core::Object, virtual public RHI::IRHISwapChain
	{
		using Super = Core::Object;

	private:
		ComPtr<IDXGISwapChain4> swapChain;

	public:
		D3D12SwapChain(IDXGISwapChain4* swapChain);
		~D3D12SwapChain() override;

		virtual bool Present();
		virtual bool PresentTest();
		virtual int32 CurrentBackBufferIndex_get() const;
	};
}