// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDescriptorTable.h"

class NullRHIDevice;

class NullRHIDescriptorTable : public RHIDescriptorTable
{
	friend NullRHIDevice;

private:
	size_t _descriptors;

private:
	NullRHIDescriptorTable(std::shared_ptr<RHIDevice> device, size_t descriptors);

public:
	virtual ~NullRHIDescriptorTable() noexcept override;

	virtual size_t GetMaxDescriptors() const override;
	virtual uint64 CopyFrom(size_t index, RHIShaderResourceViewTable* viewTable, size_t viewIndex, size_t viewCount) override;
	virtual uint64 GetDescriptor(size_t index) const override;
};