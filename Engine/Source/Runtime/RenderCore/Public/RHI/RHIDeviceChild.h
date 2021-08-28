// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class RHIDevice;

/// <summary>
/// Represents classes created by logical device.
/// </summary>
class RENDERCORE_API RHIDeviceChild : virtual public Object
{
	CLASS_BODY(RHIDeviceChild)

private:
	RHIDevice* _device = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="RHIDeviceChild"/> instance.
	/// </summary>
	/// <param name="device"> The logical device. </param>
	inline RHIDeviceChild(RHIDevice* device) : _device(device)
	{
	}

	/// <summary>
	/// Get logical device that create this instance.
	/// </summary>
	inline RHIDevice* GetDevice() const
	{
		return _device;
	}
};