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
		ComPtr<ID3D11Device> m_Device11;
		ComPtr<ID3D11DeviceContext> m_DeviceContext11;
		ComPtr<ID3D11On12Device> m_Device11On12;
		ComPtr<ID2D1Device> m_Device2D;
		ComPtr<IDWriteFactory7> m_DWrite;
		ComPtr<IWICImagingFactory> m_WIC;

	private:
		int64 m_FrameIndex = 0;
		ComPtr<ID3D12Fence> m_FrameSync;
		HANDLE m_FrameSyncEvent = NULL;

	public:
		virtual ~D3D12Graphics() noexcept override;

		virtual void Initialize() override;
		
		virtual RPtr<Window> ConfigureWindow(RPtr<GenericWindow> platformWindow) override;
		virtual RPtr<TextureImporter> CreateTextureImporter() override;

		virtual void BeginRender() override;
		virtual void EndRender() override;
		virtual int64 FrameIndex() override { return m_FrameIndex; }
	};
}
