// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Window.h"
#include "D3D12/D3D12Common.h"
#include "D3D12Window.gen.h"

namespace Ayla
{
	class GenericWindow;
	class D3D12Graphics;
	class D2D1GUI;

	ACLASS()
	class WINDOWSLAUNCH_API D3D12Window : public Window
	{
		GENERATED_BODY()

	private:
		ComPtr<ID3D12CommandQueue> m_CommandQueue;
		ComPtr<ID3D11On12Device> m_Device11On12;
		ComPtr<ID3D11DeviceContext> m_DeviceContext11;

		ComPtr<IDXGISwapChain4> m_SwapChain;
		ComPtr<ID3D12Resource> m_BackBuffers[2];
		ComPtr<ID3D11Resource> m_BackBuffersInt[2];
		ComPtr<ID2D1Bitmap1> m_Bitmaps[2];
		ComPtr<ID2D1DeviceContext> m_GUIContext;
		ComPtr<ID3D12CommandAllocator> m_Allocator[2];
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;

		APROPERTY()
		PPtr<GenericWindow> m_TargetWindow;
		APROPERTY()
		PPtr<D3D12Graphics> m_Graphics;
		APROPERTY()
		PPtr<D2D1GUI> m_GUI;

	public:
		void Initialize(RPtr<D3D12Graphics> graphics, RPtr<GenericWindow> targetWindow);

	public:
		virtual Vector2F GetSize() override;

		virtual void BeginGUI() override;
		virtual void EndGUI() override;
	};
}