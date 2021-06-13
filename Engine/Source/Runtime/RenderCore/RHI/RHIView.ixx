// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHIView;

import :RHIDeviceChild;

/// <summary>
/// Represents resource view for binding to gpu context.
/// </summary>
export class RHIView : public RHIDeviceChild
{
public:
	using Super = RHIDeviceChild;

public:
	RHIView(RHIDevice* device);
};