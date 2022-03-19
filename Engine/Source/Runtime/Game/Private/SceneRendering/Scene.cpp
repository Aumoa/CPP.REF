// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "SceneRendering/Scene.h"
#include "RHI/RHIInterfaces.h"
#include "RenderThread.h"
#include "Components/PrimitiveComponent.h"
#include "Scene/PrimitiveSceneProxy.h"

GENERATE_BODY(SScene);

SScene::SScene(IRHIDevice* InDevice) : Super()
	, Device(InDevice)
{
}

SScene::~SScene()
{
}

void SScene::Dispose()
{
	GC.SuppressFinalize(this);
}

bool SScene::AddPrimitive(SPrimitiveComponent* InPrimitive)
{
	checkf(InPrimitive->SceneProxy == nullptr, L"Primitive already have scene proxy.");

	InPrimitive->SceneProxy = InPrimitive->CreateSceneProxy();
	if (InPrimitive->SceneProxy == nullptr)
	{
		return false;
	}

	int64 Id;
	if (PrimitiveIds.empty())
	{
		Id = (int64)PrimitiveComponents.size();
		PrimitiveComponents.emplace_back(InPrimitive);
	}
	else
	{
		Id = PrimitiveIds.front();
		PrimitiveIds.pop();
		PrimitiveComponents[Id] = InPrimitive;
	}

	InPrimitive->SceneProxy->PrimitiveId = Id;

	RenderThread::Get()->EnqueueRenderThreadWork(this, [this, InPrimitive](auto)
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
		PrimitiveIds.push(Id);
		PrimitiveComponents[Id] = nullptr;

		// SceneProxy will delete in RemoveSceneProxy_RenderThread function.
		InPrimitive->SceneProxy = nullptr;

		RenderThread::Get()->EnqueueRenderThreadWork(this, [this, Id](auto)
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
	NumViewBuffers = 0;
	RequiredSize = 0;
}

void SScene::EndScene()
{
}

IRHIDevice* SScene::GetDevice()
{
	return Device;
}

SceneStructuredBuffer SScene::QueryViewBuffer(size_t InRequiredSize)
{
	if (ViewBufferSysMem.size() <= NumViewBuffers)
	{
		ViewBufferSysMem.emplace_back();
	}

	std::vector<uint8>& SysMem = ViewBufferSysMem[NumViewBuffers++];
	SysMem.resize(InRequiredSize);

	SceneStructuredBuffer Buf;
	Buf.pSysMem = SysMem.data();
	Buf.SlicePitch = InRequiredSize;
	Buf.GPUVirtualAddressOffset = RequiredSize;

	RequiredSize += (uint64)InRequiredSize;
	return Buf;
}

void SScene::ApplyViewBuffers(IRHIDeviceContext* InContext)
{
	ViewBufferCachedMemory.resize((size_t)RequiredSize);

	if (RequiredSize)
	{
		uint8* pSysMem = ViewBufferCachedMemory.data();
		for (size_t i = 0; i < ViewBufferSysMem.size(); ++i)
		{
			size_t Stride = ViewBufferSysMem[i].size();
			memcpy(pSysMem, ViewBufferSysMem[i].data(), Stride);
			pSysMem += Stride;
		}

		RHISubresourceData InitialData =
		{
			.pSysMem = ViewBufferCachedMemory.data(),
			.SysMemPitch = RequiredSize,
		};

		//if (ViewBuffer == nullptr || ViewBuffer->GetDesc().ByteWidth < (uint32)RequiredSize)
		//{
		//	RHIBufferDesc NewDesc =
		//	{
		//		.ByteWidth = (uint32)RequiredSize,
		//		.Usage = ERHIBufferUsage::Default,
		//		.InitialState = ERHIResourceStates::NonPixelShaderResource | ERHIResourceStates::PixelShaderResource,
		//		.StructureByteStride = 0,
		//		.Flags = ERHIResourceFlags::None
		//	};

		//	ViewBuffer = Device->CreateBuffer(NewDesc, &InitialData);
		//}
		//else
		//{
		//	InContext->UpdateSubresource(ViewBuffer, 0, InitialData);
		//}
	}
}

uint64 SScene::GetActualGPUVirtualAddress(const SceneStructuredBuffer& InBuffer)
{
	//if (RequiredSize)
	{
		//return ViewBuffer->GetGPUVirtualAddress() + InBuffer.GPUVirtualAddressOffset;
	}
	//else
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