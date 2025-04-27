// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Graphics.h"
#include "D3D12/D3D12Common.h"
#include "D3D12Graphics.gen.h"

namespace Ayla
{
	ACLASS()
	class WINDOWSLAUNCH_API D3D12Graphics : public Graphics
	{
		GENERATED_BODY()

	public:  // internal
		ComPtr<IDXGIFactory7> m_DXGI;
		ComPtr<ID3D12Device> m_Device;
		ComPtr<ID3D12CommandQueue> m_PrimaryCommandQueue;

	public:
		virtual void Initialize() override;
		
		virtual RPtr<Window> ConfigureWindow(RPtr<GenericWindow> platformWindow) override;
	};
}
