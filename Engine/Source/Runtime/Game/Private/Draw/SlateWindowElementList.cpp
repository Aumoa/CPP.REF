// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Draw/SlateWindowElementList.h"
#include "RHI/IRHIDeviceContext.h"
#include "RHI/IRHIBuffer.h"
#include "RHI/IRHIDevice.h"

SSlateWindowElementList::SSlateWindowElementList(SWindow* paintWindow) : Super()
	, _paintWindow(paintWindow)
{
}

void SSlateWindowElementList::SortByLayer()
{
	static auto _GetLayer = +[](const GenericSlateElement& element)
	{
		if (auto element_s = std::get_if<SlateDrawElement>(&element))
		{
			return element_s->Layer;
		}
		//else if (auto element_s = std::get_if<SlateFontElement>(&element))
		//{
		//	return element_s->Layer;
		//}
		else
		{
			check(false);
			return (int32)0;
		}
	};

	static auto _Pred = +[](const GenericSlateElement& lhs, const GenericSlateElement& rhs)
	{
		return _GetLayer(lhs) < _GetLayer(rhs);
	};

	sort(_drawElements.begin(), _drawElements.end(), _Pred);
}

void SSlateWindowElementList::Add(const SlateDrawElement& rhs)
{
	_drawElements.emplace_back(rhs);
}

//void SSlateWindowElementList::Add(const SlateFontElement& rhs)
//{
//	_drawElements.emplace_back(rhs);
//}

void SSlateWindowElementList::Clear()
{
	_drawElements.clear();
}

auto SSlateWindowElementList::GetSpan() const -> std::span<GenericSlateElement const>
{
	return _drawElements;
}

int32 SSlateWindowElementList::Num() const
{
	return (int32)_drawElements.size();
}

const SWindow* SSlateWindowElementList::GetPaintWindow() const
{
	return _paintWindow;
}

uint64 SSlateWindowElementList::CreateBuffer(IRHIDeviceContext* deviceContext, const void* drawElements, size_t sizeInBytes)
{
	IRHIDevice* device = deviceContext->GetDevice();
	uint64 cachedSize = 0;

	if (_dynamicElementBuffer)
	{
		cachedSize = _dynamicElementBuffer->GetDesc().ByteWidth;
	}

	if (cachedSize < (uint64)sizeInBytes)
	{
		// We need reallocate dynamic buffer.
		if (_dynamicElementBuffer)
		{
			IRHIDeviceContext* queue = device->GetImmediateContext();
			queue->PendingGarbageObject(_dynamicElementBuffer);
		}

		RHIBufferDesc bufferDesc = {};
		bufferDesc.ByteWidth = (uint32)sizeInBytes;
		bufferDesc.InitialState = ERHIResourceStates::GenericRead;
		bufferDesc.Usage = ERHIBufferUsage::Dynamic;
		_dynamicElementBuffer = device->CreateBuffer(bufferDesc, nullptr);
		_dynamicElementBuffer->SetOuter(this);
	}

	if (_dynamicElementBuffer)
	{
		RHISubresourceData uploadData;
		uploadData.pSysMem = drawElements;
		uploadData.SysMemPitch = sizeInBytes;
		deviceContext->UpdateSubresource(_dynamicElementBuffer, 0, uploadData);
		return _dynamicElementBuffer->GetGPUVirtualAddress();
	}
	else
	{
		return 0;
	}
}