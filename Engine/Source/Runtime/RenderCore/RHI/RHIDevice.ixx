// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "../ComPtr.h"

export module SC.Runtime.RenderCore:RHIDevice;

import SC.Runtime.Core;
import :LogRHI;
import <d3d12.h>;

/// <summary>
/// Provide interface for control all render devices.
/// </summary>
export class RHIDevice : virtual public Object
{
	ComPtr<ID3D12Device> _device;

public:
	/// <summary>
	/// Initialize new <see cref="RHIDevice"/> instance.
	/// </summary>
	RHIDevice()
	{
		HR(LogRHI, ::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&_device));
	}

	~RHIDevice() override
	{
	}
};