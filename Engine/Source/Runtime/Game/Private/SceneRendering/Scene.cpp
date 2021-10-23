// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/Scene.h"
#include "SceneRendering/SceneRenderer.h"
#include "SceneRendering/SceneRenderTarget.h"
#include "RHI/IRHIDeviceContext.h"
#include "RHI/IRHIBuffer.h"
#include "RHI/IRHIDevice.h"
#include "GameThreads/RenderThread.h"
#include "Components/PrimitiveComponent.h"
#include "Scene/PrimitiveSceneProxy.h"

SScene::SScene(IRHIDevice* InDevice) : Super()
	, _Device(InDevice)
{
}

SScene::~SScene()
{
}

bool SScene::AddPrimitive(SPrimitiveComponent* InPrimitive)
{
	check(InPrimitive->SceneProxy == nullptr);

	InPrimitive->SceneProxy = InPrimitive->CreateSceneProxy();
	if (InPrimitive->SceneProxy == nullptr)
	{
		return false;
	}

	int64 Id;
	if (_PrimitiveIds.empty())
	{
		Id = (int64)_PrimitiveComponents.size();
		_PrimitiveComponents.emplace_back(InPrimitive);
	}
	else
	{
		Id = _PrimitiveIds.front();
		_PrimitiveIds.pop();
		_PrimitiveComponents[Id] = InPrimitive;
	}

	InPrimitive->SceneProxy->PrimitiveId = Id;
	AddReferenceObject(InPrimitive);

	RenderThread::EnqueueRenderThreadWork<"AddPrimitiveInfo">([&]()
	{
		AddSceneProxy_RenderThread(InPrimitive->SceneProxy);
	});

	return true;
}

bool SScene::RemovePrimitive(SPrimitiveComponent* InPrimitive)
{
	if (InPrimitive->SceneProxy)
	{
		int64 Id = InPrimitive->SceneProxy->PrimitiveId;
		_PrimitiveIds.push(Id);
		_PrimitiveComponents[Id] = nullptr;

		// SceneProxy will delete in RemoveSceneProxy_RenderThread function.
		InPrimitive->SceneProxy = nullptr;
		RemoveReferenceObject(InPrimitive);

		RenderThread::EnqueueRenderThreadWork<"RemovePrimitiveInfo">([&]()
		{
			RemoveSceneProxy_RenderThread(Id);
		});

		return true;
	}

	return false;
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
				DestroyObject(_ViewBuffer);
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

void SScene::AddSceneProxy_RenderThread(PrimitiveSceneProxy* InPrimitive)
{
	int64 Id = InPrimitive->PrimitiveId;
	if (Primitives.size() <= (size_t)Id)
	{
		Primitives.resize(Id + 1);
	}

	Primitives[Id] = InPrimitive;
}

void SScene::RemoveSceneProxy_RenderThread(int64 InPrimitiveId)
{
	delete Primitives[InPrimitiveId];
	Primitives[InPrimitiveId] = nullptr;
}

const std::vector<PrimitiveSceneProxy*>& SScene::GetPrimitives_RenderThread()
{
	return Primitives;
}