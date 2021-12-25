// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DXGIFactoryChild.h"
#include "DirectXCommon.h"
#include "RHI/IRHIDevice.h"

class SD3D12CommandQueue;
class SD3D12ThreadAllocatorContainer;
class SD3D12ThreadDescriptorHeapContainer;
class SD3D12DescriptorHeap;

class DIRECTX_API SD3D12Device : public SDXGIFactoryChild, implements IRHIDevice
{
	GENERATED_BODY(SD3D12Device)

private:
	struct InteropDevice
	{
		ComPtr<ID3D11Device> Device;
		ComPtr<ID3D11DeviceContext> DeviceContext;
		ComPtr<ID3D11On12Device> InteropDevice;

		ComPtr<ID2D1Device> Device2D;
		ComPtr<ID2D1DeviceContext> DeviceContext2D;

		void ResetAll()
		{
			Device.Reset();
			DeviceContext.Reset();
			InteropDevice.Reset();

			Device2D.Reset();
			DeviceContext2D.Reset();
		}
	} _interop;

private:
	ComPtr<ID3D12Device> _device;

	std::mutex _allocatorLock;
	SPROPERTY(_threadAllocators)
	std::map<int64, SD3D12ThreadAllocatorContainer*> _threadAllocators;
	std::mutex _heapsLock;
	SPROPERTY(_threadSrvHeaps)
	std::map<int64, SD3D12ThreadDescriptorHeapContainer*> _threadSrvHeaps;
	SPROPERTY(_threadSamplerHeaps)
	std::map<int64, SD3D12ThreadDescriptorHeapContainer*> _threadSamplerHeaps;
	
	uint64 _fenceValue = 0;
	SPROPERTY(_immCon)
	SD3D12CommandQueue* _immCon = nullptr;

public:
	SD3D12Device(SDXGIFactory* InFactory, ComPtr<ID3D12Device> device);

	virtual IRHIDeviceContext* GetImmediateContext() override;
	virtual IRHIDeviceContext* CreateDeviceContext() override;
	virtual IRHIDeviceContext2D* CreateDeviceContext2D() override;
	virtual IRHITexture2D* CreateTexture2D(const RHITexture2DDesc& Desc, const RHISubresourceData* InitialData) override;
	virtual IRHIBuffer* CreateBuffer(const RHIBufferDesc& Desc, const RHISubresourceData* InitialData) override;
	virtual IRHIShader* CompileShader(SMaterial* Material) override;
	virtual IRHIRenderTargetView* CreateRenderTargetView(int32 Count) override;
	virtual IRHIDepthStencilView* CreateDepthStencilView(int32 Count) override;
	virtual IRHIShaderResourceView* CreateShaderResourceView(int32 Count) override;
	virtual IRHISolidColorBrush* CreateSolidColorBrush(const Color& InColor, float InOpacity) override;
	virtual IRHIBitmap* CreateBitmapFromTexture2D(IRHITexture2D* InTexture) override;

	virtual void BeginFrame() override;
	virtual void EndFrame() override;
	virtual void FlushCommands() override;

	ID3D12CommandAllocator* GetThreadPrimaryAllocator();
	SD3D12DescriptorHeap* GetThreadPrimarySrvHeap(int32 Count);
	SD3D12DescriptorHeap* GetThreadPrimarySamplerHeap(int32 Count);
	void MarkPendingAllocatorAndHeaps(uint64 fenceValue);

protected:
	virtual void Dispose(bool bDisposing) override;

public:
	DECLARE_GETTER(ID3D12Device, _device);
	DECLARE_GETTER(ID2D1Device, _interop.Device2D);
	DECLARE_GETTER(ID3D11On12Device, _interop.InteropDevice);
	DECLARE_GETTER(ID2D1DeviceContext, _interop.DeviceContext2D);

private:
	void AllocateCommandQueue();
	void CreateInteropDevice();
};