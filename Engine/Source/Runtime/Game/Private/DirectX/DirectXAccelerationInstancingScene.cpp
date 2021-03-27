// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXAccelerationInstancingScene.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "DirectX/DirectXDeviceContext.h"
#include "Logging/LogMacros.h"
#include "SceneRendering/PrimitiveSceneProxy.h"

using namespace std;

DirectXAccelerationInstancingScene::DirectXAccelerationInstancingScene(DirectXDeviceBundle* deviceBundle) : Super(deviceBundle)
	, device(deviceBundle->GetDevice())
	, lastCount(0)

	, actualInstanceCount(0)

	, bNeedRebuild(false)
	, bAllowUpdate(false)
{

}

DirectXAccelerationInstancingScene::~DirectXAccelerationInstancingScene()
{

}

void DirectXAccelerationInstancingScene::UpdateScene()
{
	if (proxies.size() == 0)
	{
		return;
	}

	D3D12_RAYTRACING_INSTANCE_DESC* instances = nullptr;
	size_t instances_size = 0;
	HR(instanceBuffer->Map(0, nullptr, (void**)&instances));

	// Write instance description to gpu-based buffer.
	actualInstanceCount = 0;
	uint32 shaderRecords = 0;
	for (size_t i = 0; i < proxies.size(); ++i)
	{
		// Without null referenced instance.
		if (proxies[i] != nullptr && proxies[i]->PrimitiveAccelerationPtr != 0)
		{
			instances[instances_size++] = GetRaytracingInstanceDesc(shaderRecords, proxies[i]);
			actualInstanceCount += 1;
		}
	}

	instanceBuffer->Unmap(0, nullptr);
}

void DirectXAccelerationInstancingScene::BuildScene(DirectXDeviceContext* deviceContext)
{
	if (actualInstanceCount == 0)
	{
		return;
	}

	ID3D12GraphicsCommandList4* inCommandList = deviceContext->GetCommandList();
	if (oldAccelerationStructure.IsValid)
	{
		// Clanup old acceleration structure when new build is started.
		deviceContext->AddPendingReference(move(oldAccelerationStructure));
	}

	if (bNeedRebuild)
	{
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC buildDesc = { };
		buildDesc.DestAccelerationStructureData = accelerationStructure->GetGPUVirtualAddress();
		buildDesc.Inputs =
		{
			D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL,
			D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE,
			(UINT)actualInstanceCount,
			D3D12_ELEMENTS_LAYOUT_ARRAY
		};
		buildDesc.Inputs.InstanceDescs = instanceBuffer->GetGPUVirtualAddress();
		buildDesc.ScratchAccelerationStructureData = scratchBuffer->GetGPUVirtualAddress();

		inCommandList->BuildRaytracingAccelerationStructure(&buildDesc, 0, nullptr);

		bNeedRebuild = false;
	}
}

void DirectXAccelerationInstancingScene::AddInstance(size_t index, const PrimitiveSceneProxy* instanceDesc)
{
	AddInstanceDeferred(index, instanceDesc);
	FinishEditInstance();
}

void DirectXAccelerationInstancingScene::RemoveInstance(size_t index)
{
	RemoveInstanceDeferred(index);
	FinishEditInstance();
}

void DirectXAccelerationInstancingScene::AddInstanceDeferred(size_t index, const PrimitiveSceneProxy* instanceDesc)
{
	if (proxies.size() <= index)
	{
		proxies.resize(index + 1);
	}

	if (proxies[index] != nullptr)
	{
		SE_LOG(LogDirectX, Error, L"instanceDescs[{0}] is not null. Overwrite is not allowed.", index);
		return;
	}

	proxies[index] = instanceDesc;
}

void DirectXAccelerationInstancingScene::RemoveInstanceDeferred(size_t index)
{
	if (proxies.size() > index)
	{
		SE_LOG(LogDirectX, Error, L"{0} is not valid index.", index);
		return;
	}

	proxies[index] = nullptr;
}

void DirectXAccelerationInstancingScene::FinishEditInstance()
{
	CheckAndReallocate(proxies.size());
	bNeedRebuild = true;
}

uint64 DirectXAccelerationInstancingScene::GetRaytracingAccelerationStructureBuffer() const
{
	if (accelerationStructure.IsValid)
	{
		return accelerationStructure->GetGPUVirtualAddress();
	}
	else
	{
		// There has no any instances.
		return 0;
	}
}

void DirectXAccelerationInstancingScene::CheckAndReallocate(size_t desiredCount)
{
	if (desiredCount <= lastCount)
	{
		return;
	}

	// Save old acceleration structure if it is now using.
	if (!oldAccelerationStructure.IsValid)
	{
		oldAccelerationStructure = move(accelerationStructure);
	}

	// Update inputs information.
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs{ };
	inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
	inputs.NumDescs = (UINT)desiredCount;
	inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;

	// Query building size.
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO prebuildInfo;
	device->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &prebuildInfo);

	// Ready buffers for compute raytracing acceleration structure.
	D3D12_RESOURCE_DESC bufferDesc = { };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = prebuildInfo.ResultDataMaxSizeInBytes;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	D3D12_HEAP_PROPERTIES heapProp = { };
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	HR(device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, nullptr, IID_PPV_ARGS(&accelerationStructure)));
	SetNameAuto(accelerationStructure);

	bufferDesc.Width = prebuildInfo.ScratchDataSizeInBytes;
	HR(device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&scratchBuffer)));
	SetNameAuto(scratchBuffer);

	// Create instance desc gpu-based buffer.
	bufferDesc.Width = sizeof(D3D12_RAYTRACING_INSTANCE_DESC) * desiredCount;
	bufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	HR(device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&instanceBuffer)));

	lastCount = desiredCount;
}

D3D12_RAYTRACING_INSTANCE_DESC DirectXAccelerationInstancingScene::GetRaytracingInstanceDesc(uint32& shaderRecords, const PrimitiveSceneProxy* sceneProxy) const
{
	D3D12_RAYTRACING_INSTANCE_DESC instance = { };
	instance.InstanceID = sceneProxy->PrimitiveId;
	instance.InstanceMask = 0xFF;
	instance.InstanceContributionToHitGroupIndex = shaderRecords;
	instance.Flags = D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
	instance.AccelerationStructure = sceneProxy->PrimitiveAccelerationPtr;

	Matrix4x4 matrix = sceneProxy->PrimitiveTransform.Matrix;
	instance.Transform[0][0] = matrix._11;
	instance.Transform[0][1] = matrix._21;
	instance.Transform[0][2] = matrix._31;
	instance.Transform[0][3] = matrix._41;

	instance.Transform[1][0] = matrix._12;
	instance.Transform[1][1] = matrix._22;
	instance.Transform[1][2] = matrix._32;
	instance.Transform[1][3] = matrix._42;

	instance.Transform[2][0] = matrix._13;
	instance.Transform[2][1] = matrix._23;
	instance.Transform[2][2] = matrix._33;
	instance.Transform[2][3] = matrix._43;

	shaderRecords += (uint32)sceneProxy->InstanceShaderRecord.size();
	return instance;
}