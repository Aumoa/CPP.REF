// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Graphics.h"
#include "D3D12Common.h"
#include "D3D12Graphics.gen.h"

namespace Ayla
{
	ACLASS()
	class DIRECT3D12_API D3D12Graphics : public Graphics
	{
		GENERATED_BODY()

	private:
		ComPtr<IDXGIFactory5> m_DXGI;

	public:
		ACONSTRUCTOR()
		D3D12Graphics();
		virtual ~D3D12Graphics() noexcept override;
	};
}