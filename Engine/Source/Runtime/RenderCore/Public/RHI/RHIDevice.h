// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ComPtr.h"
#include "RHIStructures.h"

struct IDXGIFactory2;
struct ID3D12Device;
struct IRHIImageSourceView;
class SRHICommandQueue;
class SRHIResource;
class SRHITexture2D;
class SRHIDynamicTexture2D;

/// <summary>
/// Provide interface for control all render devices.
/// </summary>
class RENDERCORE_API SRHIDevice : implements SObject
{
	GENERATED_BODY(SRHIDevice)

private:
	const uint8 _bDebug : 1;

	ComPtr<IDXGIFactory2> _factory;
	ComPtr<ID3D12Device> _device;
	SRHICommandQueue* _queue = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="SRHIDevice"/> instance.
	/// </summary>
	SRHIDevice(bool bDebug = false);
	~SRHIDevice() override;

	/// <summary>
	/// Get primary queue.
	/// </summary>
	SRHICommandQueue* GetPrimaryQueue() const { return _queue; }

	/// <summary>
	/// Create immutable buffer.
	/// </summary>
	SRHIResource* CreateImmutableBuffer(ERHIResourceStates initialState, const uint8* buffer, size_t length);

	/// <summary>
	/// Create dynamic buffer.
	/// </summary>
	SRHIResource* CreateDynamicBuffer(size_t length);

	SRHITexture2D* CreateTexture2D(ERHIResourceStates initialState, ERHIPixelFormat format, uint32 width, uint32 height, std::optional<RHITexture2DClearValue> clearValue, ERHIResourceFlags flags = ERHIResourceFlags::None);
	SRHIDynamicTexture2D* CreateDynamicTexture2D(ERHIResourceStates initialState, ERHIPixelFormat format, uint32 width, uint32 height, std::optional<RHITexture2DClearValue> clearValue, ERHIResourceFlags flags = ERHIResourceFlags::None);
	SRHITexture2D* CreateTexture2DFromImage(IRHIImageSourceView* imageSource);

public /*internal*/:
	IDXGIFactory2* GetFactory() const { return _factory.Get(); }
	ID3D12Device* GetDevice() const { return _device.Get(); }

private:
	void InitializeDebug();
	void InitializeCOM();
	void InitializeDXGI();
	void InitializeD3D12();
};