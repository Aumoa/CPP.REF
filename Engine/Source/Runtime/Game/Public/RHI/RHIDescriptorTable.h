// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceResource.h"

class RHIShaderResourceViewTable;

class GAME_API RHIDescriptorTable : public RHIDeviceResource
{
protected:
	RHIDescriptorTable(std::shared_ptr<RHIDevice> device);

public:
	virtual ~RHIDescriptorTable() noexcept override;

	virtual size_t GetMaxDescriptors() const = 0;
	virtual uint64 CopyFrom(size_t index, RHIShaderResourceViewTable* viewTable, size_t viewIndex, size_t viewCount) = 0;
	virtual uint64 GetDescriptor(size_t index) const = 0;
};