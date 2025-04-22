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

	private:
		ComPtr<ID3D12Device> m_Device;

	public:
		virtual void Initialize() override;
	};
}
