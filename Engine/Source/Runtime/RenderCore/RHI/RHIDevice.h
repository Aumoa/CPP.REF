// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ComPtr.h"
#include "RHIEnums.h"

struct IDXGIFactory2;
struct ID3D12Device;
class RHICommandQueue;
class RHIResource;

/// <summary>
/// Provide interface for control all render devices.
/// </summary>
class RHIDevice : virtual public Object
{
public:
	using Super = Object;

private:
	const uint8 _bDebug : 1;

	ComPtr<IDXGIFactory2> _factory;
	ComPtr<ID3D12Device> _device;
	RHICommandQueue* _queue = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="RHIDevice"/> instance.
	/// </summary>
	RHIDevice(bool bDebug = false);
	~RHIDevice() override;

	/// <summary>
	/// Get primary queue.
	/// </summary>
	RHICommandQueue* GetPrimaryQueue() const { return _queue; }

	/// <summary>
	/// Create immutable buffer.
	/// </summary>
	RHIResource* CreateImmutableBuffer(ERHIResourceStates initialState, const uint8* buffer, size_t length);

	/// <summary>
	/// Create dynamic buffer.
	/// </summary>
	RHIResource* CreateDynamicBuffer(size_t length);

public /*internal*/:
	IDXGIFactory2* GetFactory() const { return _factory.Get(); }
	ID3D12Device* GetDevice() const { return _device.Get(); }

private:
	void InitializeDebug();
	void InitializeCOM();
	void InitializeDXGI();
	void InitializeD3D12();
};