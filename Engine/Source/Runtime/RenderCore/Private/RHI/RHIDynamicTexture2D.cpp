// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "RHI/RHIDynamicTexture2D.h"
#include "RHI/RHIDevice.h"
#include "RHI/RHICommandQueue.h"
#include "RHI/LogRHI.h"
#include "RHI/RHIDeviceContext.h"
#include "InternalComPtr.h"

SRHIDynamicTexture2D::SRHIDynamicTexture2D(SRHIDevice* device, ID3D12Resource* texture, ID3D12Resource* uploadHeap, const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& layout)
	: Super(device, texture)
	, _texture(texture)
	, _layout(layout)
	, _uploadHeap(uploadHeap)
{
	HR(LogRHI, _uploadHeap->Map(0, nullptr, &_pointer));
	_deviceContext = NewObject<SRHIDeviceContext>(device);
}

SRHIDynamicTexture2D::~SRHIDynamicTexture2D()
{
}

void SRHIDynamicTexture2D::CloseUploadBuffer()
{
	if (_uploadHeap)
	{
		SRHICommandQueue* queue = GetDevice()->GetPrimaryQueue();
		queue->AddGarbageObject(queue->GetLastSignal(), _uploadHeap.Detach());
		queue->AddGarbageObject(queue->GetLastSignal(), _deviceContext);

		_pointer = nullptr;
		_deviceContext = nullptr;
	}
}

void SRHIDynamicTexture2D::UpdatePixels(const void* buffer, int32 pixelStride, const Vector2N& size, const Vector2N& location)
{
	check(_uploadHeap);
	check(_pointer);

	int32 rowpitch = pixelStride * size.X;

	for (int32 i = 0; i < size.Y; ++i)
	{
		const void* sourceBuf = reinterpret_cast<const int8*>(buffer) + size.X * i;
		void* destBuf = reinterpret_cast<int8*>(_pointer) + (size_t)_layout.Footprint.RowPitch * (i + location.Y) + location.X * pixelStride;
		memcpy(destBuf, sourceBuf, rowpitch);
	}

	RefreshUpdateBox(location.X, location.Y, location.X + size.X, location.Y + size.Y);
}

void SRHIDynamicTexture2D::Apply()
{
	check(_deviceContext);

	if (!HasUpdateBox())
	{
		return;
	}

	D3D12_TEXTURE_COPY_LOCATION locationSrc = {};
	locationSrc.pResource = _uploadHeap.Get();
	locationSrc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	locationSrc.PlacedFootprint = _layout;

	D3D12_TEXTURE_COPY_LOCATION locationDst = {};
	locationDst.pResource = _texture;
	locationDst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	locationDst.SubresourceIndex = 0;

	D3D12_RESOURCE_BARRIER barrier =
	{
		.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		.Transition =
		{
			.pResource = _texture,
			.Subresource = 0,
			.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST,
		}
	};

	D3D12_BOX sourceBox;
	sourceBox.left = _updateBoxLeft;
	sourceBox.top = _updateBoxTop;
	sourceBox.right = _updateBoxRight;
	sourceBox.bottom = _updateBoxBottom;
	sourceBox.front = 0;
	sourceBox.back = 1;

	_deviceContext->Begin();

	// Copy pixels from update buffer to texture.
	auto* commandList = static_cast<ID3D12GraphicsCommandList*>(_deviceContext->GetCommandList());
	commandList->ResourceBarrier(1, &barrier);
	//commandList->CopyTextureRegion(&locationDst, _updateBoxLeft, _updateBoxTop, 0, &locationSrc, &sourceBox);
	commandList->CopyTextureRegion(&locationDst, 0, 0, 0, &locationSrc, nullptr);

	// Restore barrier.
	std::swap(barrier.Transition.StateBefore, barrier.Transition.StateAfter);
	commandList->ResourceBarrier(1, &barrier);

	// Flushing graphics commands.
	_deviceContext->End();
	SRHICommandQueue* queue = GetDevice()->GetPrimaryQueue();
	queue->ExecuteDeviceContext(_deviceContext);

	ResetUpdateBox();
}

void SRHIDynamicTexture2D::RefreshUpdateBox(int32 left, int32 top, int32 right, int32 bottom)
{
	if (_updateBoxLeft == -1)
	{
		_updateBoxLeft = left;
		_updateBoxTop = top;
		_updateBoxRight = right;
		_updateBoxBottom = bottom;
	}
	else
	{
		_updateBoxLeft = std::min(left, _updateBoxLeft);
		_updateBoxTop = std::min(top, _updateBoxTop);
		_updateBoxRight = std::max(right, _updateBoxRight);
		_updateBoxBottom = std::max(bottom, _updateBoxBottom);
	}
}

void SRHIDynamicTexture2D::ResetUpdateBox()
{
	_updateBoxLeft = -1;
}