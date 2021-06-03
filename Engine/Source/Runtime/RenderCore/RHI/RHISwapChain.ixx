// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHISwapChain;

import SC.Runtime.Core;
import SC.Runtime.RenderCore.Internal;
import :ComPtr;

export class RHIDevice;
export struct IWindowView;

export class RHISwapChain : virtual public Object
{
public:
	using Super = Object;

private:
	ComPtr<IDXGISwapChain> _swapChain;

public:
	RHISwapChain(RHIDevice* device, IWindowView* view);
	~RHISwapChain() override;
};