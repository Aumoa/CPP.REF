// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHIDevice;

import SC.Runtime.Core;
import SC.Runtime.RenderCore.Internal;
import :LogRHI;
import :ComPtr;

using enum ELogVerbosity;

/// <summary>
/// Provide interface for control all render devices.
/// </summary>
export class RHIDevice : virtual public Object
{
public:
	using Super = Object;

private:
	const uint8 _bDebug : 1;

	ComPtr<IDXGIFactory2> _factory;
	ComPtr<ID3D12Device> _device;

public:
	/// <summary>
	/// Initialize new <see cref="RHIDevice"/> instance.
	/// </summary>
	RHIDevice(bool bDebug = false);
	~RHIDevice() override;

public /*internal*/ :
	IDXGIFactory2* GetFactory() const { return _factory.Get(); }
	ID3D12Device* GetDevice() const { return _device.Get(); }

private:
	void InitializeDebug();
	void InitializeCOM();
	void InitializeDXGI();
	void InitializeD3D12();
};