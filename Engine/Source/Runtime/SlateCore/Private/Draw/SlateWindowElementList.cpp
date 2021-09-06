// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Draw/SlateWindowElementList.h"
#include "RHI/RHIDeviceContext.h"
#include "RHI/RHIDevice.h"
#include "RHI/RHIResource.h"
#include "RHI/RHICommandQueue.h"

SlateWindowElementList::SlateWindowElementList(const SWindow* paintWindow) : Super()
	, _paintWindow(paintWindow)
{
}

void SlateWindowElementList::SortByLayer()
{
	auto _Pred = [](const SlateDrawElement& lhs, const SlateDrawElement& rhs)
	{
		return lhs.Layer - rhs.Layer;
	};

	sort(_drawElements.begin(), _drawElements.end(), _Pred);
}

void SlateWindowElementList::Add(const SlateDrawElement& rhs)
{
	_drawElements.emplace_back(rhs);
}

void SlateWindowElementList::Clear()
{
	_drawElements.clear();
}

std::span<SlateDrawElement const> SlateWindowElementList::GetSpan() const
{
	return _drawElements;
}

int32 SlateWindowElementList::Num() const
{
	return (int32)_drawElements.size();
}

const SWindow* SlateWindowElementList::GetPaintWindow() const
{
	return _paintWindow;
}

uint64 SlateWindowElementList::CreateBuffer(RHIDeviceContext* deviceContext, const void* drawElements, size_t sizeInBytes)
{
	RHIDevice* device = deviceContext->GetDevice();
	uint64 cachedSize = 0;

	if (_dynamicElementBuffer)
	{
		cachedSize = _dynamicElementBuffer->GetBufferSize();
	}

	if (cachedSize < (uint64)sizeInBytes)
	{
		// We need reallocate dynamic buffer.
		if (_dynamicElementBuffer)
		{
			RHICommandQueue* queue = device->GetPrimaryQueue();
			queue->AddGarbageObject(queue->GetLastSignal(), _dynamicElementBuffer);
		}

		_dynamicElementBuffer = device->CreateDynamicBuffer((uint64)sizeInBytes);
		_dynamicElementBuffer->SetOuter(this);
	}

	if (_dynamicElementBuffer)
	{
		void* ptr = _dynamicElementBuffer->GetMappingPointer();
		memcpy(ptr, drawElements, sizeInBytes);

		return _dynamicElementBuffer->GetGPUVirtualAddress();
	}
	else
	{
		return 0;
	}
}