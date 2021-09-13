// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SRHIDevice;

/// <summary>
/// Represents classes created by logical device.
/// </summary>
class RENDERCORE_API SRHIDeviceChild : virtual public SObject
{
	GENERATED_BODY(SRHIDeviceChild)

private:
	SRHIDevice* _device = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="SRHIDeviceChild"/> instance.
	/// </summary>
	/// <param name="device"> The logical device. </param>
	inline SRHIDeviceChild(SRHIDevice* device) : _device(device)
	{
	}

	/// <summary>
	/// Get logical device that create this instance.
	/// </summary>
	inline SRHIDevice* GetDevice() const
	{
		return _device;
	}
};