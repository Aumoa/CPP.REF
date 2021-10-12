// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/Scene.h"
#include "SceneRendering/SceneRenderer.h"
#include "SceneRendering/SceneRenderTarget.h"
#include "RHI/IRHIDeviceContext.h"
#include "RHI/IRHIBuffer.h"
#include "RHI/IRHIDevice.h"

SScene::SScene(IRHIDevice* InDevice) : Super()
	, _Device(InDevice)
{
}

SScene::~SScene()
{
}

int64 SScene::AddPrimitive(const PrimitiveSceneInfo& InPrimitive)
{
	if (_PrimitiveIds.empty())
	{
		int64 Id = (int64)_Primitives.size();
		_Primitives.emplace_back(InPrimitive);
		return Id;
	}
	else
	{
		int64 Id = _PrimitiveIds.front();
		_PrimitiveIds.pop();
		_Primitives[Id] = InPrimitive;
		return Id;
	}
}

bool SScene::RemovePrimitive(int64 InPrimitiveId)
{
	if ((int64)_Primitives.size() > InPrimitiveId && _Primitives[InPrimitiveId].has_value())
	{
		_Primitives[InPrimitiveId].reset();
		_PrimitiveIds.push(InPrimitiveId);
		return true;
	}

	return false;
}

void SScene::UpdatePrimitive(int64 InPrimitiveId, const PrimitiveSceneInfo& InPrimitive)
{
	_Primitives[InPrimitiveId] = InPrimitive;
}

int64 SScene::AddLight(const LightSceneInfo& InLight)
{
	if (_LightIds.empty())
	{
		int64 Id = (int64)_Lights.size();
		_Lights.emplace_back(InLight);
		return Id;
	}
	else
	{
		int64 Id = _LightIds.front();
		_LightIds.pop();
		_Lights[Id] = InLight;
		return Id;
	}
}

bool SScene::RemoveLight(int64 InPrimitiveId)
{
	if ((int64)_Lights.size() > InPrimitiveId && _Lights[InPrimitiveId].has_value())
	{
		_Lights[InPrimitiveId].reset();
		_LightIds.push(InPrimitiveId);
		return true;
	}

	return false;
}

void SScene::UpdateLight(int64 InPrimitiveId, const LightSceneInfo& InLight)
{
	_Lights[InPrimitiveId] = InLight;
}

void SScene::BeginScene()
{
	// Resize view buffers manually for keep capacity.
	_NumViewBuffers = 0;
	_RequiredSize = 0;
}

void SScene::EndScene()
{
}

IRHIDevice* SScene::GetDevice()
{
	return _Device;
}

SceneStructuredBuffer SScene::QueryViewBuffer(size_t InRequiredSize)
{
	if (_ViewBufferSysMem.size() <= _NumViewBuffers)
	{
		_ViewBufferSysMem.emplace_back();
	}

	std::vector<uint8>& SysMem = _ViewBufferSysMem[_NumViewBuffers++];
	SysMem.resize(InRequiredSize);

	SceneStructuredBuffer Buf;
	Buf.pSysMem = SysMem.data();
	Buf.SlicePitch = InRequiredSize;
	Buf.GPUVirtualAddressOffset = _RequiredSize;

	_RequiredSize += (uint64)InRequiredSize;
	return Buf;
}

void SScene::ApplyViewBuffers(IRHIDeviceContext* InContext)
{
	_ViewBufferCachedMemory.resize((size_t)_RequiredSize);

	if (_RequiredSize)
	{
		uint8* pSysMem = _ViewBufferCachedMemory.data();
		for (size_t i = 0; i < _ViewBufferSysMem.size(); ++i)
		{
			size_t Stride = _ViewBufferSysMem[i].size();
			memcpy(pSysMem, _ViewBufferSysMem[i].data(), Stride);
			pSysMem += Stride;
		}

		RHISubresourceData InitialData =
		{
			.pSysMem = _ViewBufferCachedMemory.data(),
			.SysMemPitch = _RequiredSize,
		};

		if (_ViewBuffer == nullptr || _ViewBuffer->GetDesc().ByteWidth < (uint32)_RequiredSize)
		{
			if (_ViewBuffer)
			{
				DestroySubobject(_ViewBuffer);
				_ViewBuffer = nullptr;
			}

			RHIBufferDesc NewDesc =
			{
				.ByteWidth = (uint32)_RequiredSize,
				.Usage = ERHIBufferUsage::Default,
				.InitialState = ERHIResourceStates::NonPixelShaderResource | ERHIResourceStates::PixelShaderResource,
				.StructureByteStride = 0,
				.Flags = ERHIResourceFlags::None
			};

			_ViewBuffer = _Device->CreateBuffer(NewDesc, &InitialData);
			_ViewBuffer->SetOuter(this);
		}
		else
		{
			InContext->UpdateSubresource(_ViewBuffer, 0, InitialData);
		}
	}
}

uint64 SScene::GetActualGPUVirtualAddress(const SceneStructuredBuffer& InBuffer)
{
	if (_RequiredSize)
	{
		return _ViewBuffer->GetGPUVirtualAddress() + InBuffer.GPUVirtualAddressOffset;
	}
	else
	{
		return 0;
	}
}