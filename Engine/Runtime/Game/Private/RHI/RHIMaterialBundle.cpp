// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "RHIMaterialBundle.h"

#include "Engine.h"
#include "RHI/IRHIDeviceBundle.h"
#include "RHI/IRHIDeferredCommandList.h"
#include "RHI/RHIResourceGC.h"
#include "RHI/IRHIResource.h"
#include "Materials/Material.h"

using namespace std;

RHIMaterialBundle::RHIMaterialBundle() : Super()
	, currSize(0)
{

}

RHIMaterialBundle::~RHIMaterialBundle()
{

}

void RHIMaterialBundle::InitializeBundle()
{
	Resize(CapacityUnit);
}

void RHIMaterialBundle::ReleaseBundle()
{

}

void RHIMaterialBundle::Commit(IRHICommandList* inCommandList)
{
	if (dirtys.empty())
	{
		return;
	}

	using range = pair<uint64, uint64>;
	vector<range> updateRanges;
	updateRanges.reserve(dirtys.size());

	for (auto& dirty : dirtys)
	{
		dirty->ResolveDirtyState();

		// Fast-single merge for all components are dirty.
		range myRange = GetVirtualAddressRange(dirty->Index);
		if (!updateRanges.empty() && myRange.first <= updateRanges.back().second)
		{
			updateRanges.back().second = myRange.second;
		}
		else
		{
			updateRanges.emplace_back(myRange);
		}
	}

	vector<range> mergedRanges = MergeRanges(updateRanges);

	auto before = ERHIResourceStates::NON_PIXEL_SHADER_RESOURCE;
	auto after = ERHIResourceStates::COPY_DEST;

	inCommandList->ResourceTransition(immutableBuffer.Get(), before, after);
	for (auto& range : mergedRanges)
	{
		uint64 numBytes = range.second - range.first;
		inCommandList->CopyBufferRegion(immutableBuffer.Get(), range.first, dynamicBuffer.Get(), range.first, numBytes);
	}
	inCommandList->ResourceTransition(immutableBuffer.Get(), after, before);
}

TRefPtr<MaterialInterface> RHIMaterialBundle::CreateMaterial()
{
	auto issue = Issue();
	auto material = NewObject<Material>(issue, this);
	materials[issue] = material.Get();
	return material;
}

uint64 RHIMaterialBundle::GetMaterialsBufferVirtualAddress() const
{
	return immutableBuffer->GetVirtualAddress();
}

MaterialInterface* RHIMaterialBundle::FindMaterialFromIndex(uint16 index) const
{
	return materials[index];
}

void RHIMaterialBundle::EnqueueDirty(MaterialInterface* inQuery)
{
	dirtys.emplace_back(inQuery);
}

void* RHIMaterialBundle::GetUploadBufferPointer(uint16 inMaterialIndex)
{
	char* ptr = (char*)dynamicBuffer->GetMappingAddress();
	return ptr + SizeofMaterial * inMaterialIndex;
}

uint64 RHIMaterialBundle::GetBufferVirtualAddress() const
{
	return immutableBuffer->GetVirtualAddress();
}

void RHIMaterialBundle::ReleaseMaterial(uint16 index)
{
	if (materials[index] != nullptr)
	{
		materials[index] = nullptr;
		reserved.emplace(index);
	}
}

uint16 RHIMaterialBundle::Issue()
{
	if (reserved.empty())
	{
		// Expand material buffer size.
		Resize(currSize + CapacityUnit);
	}

	uint16 pop = reserved.front();
	reserved.pop();

	return pop;
}

void RHIMaterialBundle::Resize(size_t inNewSize)
{
	if (inNewSize <= currSize)
	{
		// Shrink is not allowed.
		return;
	}

	auto device = GEngine.DeviceBundle;

	size_t slack = inNewSize - currSize;
	TRefPtr<IRHIResource> oldDynamic = dynamicBuffer;

	immutableBuffer = device->CreateImmutableBuffer(SizeofMaterial * inNewSize, ERHIResourceStates::NON_PIXEL_SHADER_RESOURCE);
	dynamicBuffer = device->CreateDynamicBuffer(SizeofMaterial * inNewSize);

	// Copy previous data to new buffer.
	if (oldDynamic.IsValid)
	{
		memcpy(dynamicBuffer->GetMappingAddress(), oldDynamic->GetMappingAddress(), SizeofMaterial * currSize);
	}

	// Set resource state dirty for all materials.
	for (auto& item : materials)
	{
		item->SetMarkDirty(EMaterialDirtyMask::UpdateBuffer);
	}
	dirtys = materials;

	// Enqueue reserved index buffer.
	for (uint16 i = (uint16)currSize; i < (uint16)inNewSize; ++i)
	{
		reserved.push(i);
	}

	currSize = inNewSize;
	materials.resize(currSize);
}

pair<uint64, uint64> RHIMaterialBundle::GetVirtualAddressRange(uint16 index) const
{
	auto first = (uint64)index * (uint64)SizeofMaterial;
	auto second = first + (uint64)SizeofMaterial;
	return { first, second };
}

vector<pair<uint64, uint64>> RHIMaterialBundle::MergeRanges(const vector<pair<uint64, uint64>>& inRanges) const
{
	vector<pair<uint64, uint64>> results;
	results.reserve(inRanges.size());

	auto it = inRanges.begin();
	pair<uint64, uint64> current = *it;

	while (it != inRanges.end())
	{
		if (current.second >= it->first)
		{
			current.second = max(current.second, it->second);
		}
		else
		{
			results.emplace_back(current.first, current.second);
			current = *it;
		}

		it++;
	}
	results.emplace_back(current.first, current.second);

	return results;
}