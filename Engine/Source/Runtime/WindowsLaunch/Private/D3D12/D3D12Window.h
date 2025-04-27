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

	ACLASS()
	class WINDOWSLAUNCH_API D3D12Window : public Window
	{
		GENERATED_BODY()

	private:
		ComPtr<IDXGISwapChain1> m_SwapChain;

	public:
		void Initialize(RPtr<D3D12Graphics> graphics, RPtr<GenericWindow> targetWindow);

	public:
		virtual void Present() override;
	};
}