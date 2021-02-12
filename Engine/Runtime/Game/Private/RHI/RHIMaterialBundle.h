// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIMaterialBundle.h"

#include "RHI/RHICommon.h"

class MaterialInstanceProxy;
interface IRHIResource;
interface IRHIDeferredCommandList;
interface IRHICommandList;

class RHIMaterialBundle : virtual public Object, virtual public IRHIMaterialBundle
{
public:
	using Super = Object;
	using This = RHIMaterialBundle;

private:
	static constexpr const size_t SizeofMaterial = sizeof(RHIMaterial);
	static constexpr const size_t CapacityUnit = 128;

	TRefPtr<IRHIResource> immutableBuffer;
	TRefPtr<IRHIResource> dynamicBuffer;
	std::queue<uint16> reserved;
	std::vector<MaterialInterface*> materials;
	size_t currSize;

	std::vector<MaterialInterface*> dirtys;

public:
	RHIMaterialBundle();
	~RHIMaterialBundle() override;

	virtual void InitializeBundle();
	virtual void ReleaseBundle();
	virtual void Commit(IRHICommandList* inCommandList);

	virtual TRefPtr<MaterialInterface> CreateMaterial();
	virtual uint64 GetMaterialsBufferVirtualAddress() const;

	void EnqueueDirty(MaterialInterface* inQuery);
	void* GetUploadBufferPointer(uint16 inMaterialIndex);
	uint64 GetBufferVirtualAddress() const;

private:
	uint16 Issue();
	void Resize(size_t inNewSize);

	std::pair<uint64, uint64> GetVirtualAddressRange(uint16 index) const;
	std::vector<std::pair<uint64, uint64>> MergeRanges(const std::vector<std::pair<uint64, uint64>>& inRanges) const;
};