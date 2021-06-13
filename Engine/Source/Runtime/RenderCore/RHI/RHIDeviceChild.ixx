// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHIDeviceChild;

import SC.Runtime.Core;

export class RHIDevice;

/// <summary>
/// Represents classes created by logical device.
/// </summary>
export class RHIDeviceChild : virtual public Object
{
public:
	using Super = Object;

private:
	RHIDevice* _device = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="RHIDeviceChild"/> instance.
	/// </summary>
	/// <param name="device"> The logical device. </param>
	RHIDeviceChild(RHIDevice* device) : _device(device)
	{
	}

	/// <summary>
	/// Get logical device that create this instance.
	/// </summary>
	RHIDevice* GetDevice() const
	{
		return _device;
	}
};