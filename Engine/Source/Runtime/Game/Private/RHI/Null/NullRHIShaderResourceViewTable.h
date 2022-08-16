// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIShaderResourceViewTable.h"

class NullRHIDevice;
enum class ERHIShaderResourceViewType;

class NullRHIShaderResourceViewTable : public RHIShaderResourceViewTable
{
	friend NullRHIDevice;

private:
	ERHIShaderResourceViewType _type;
	size_t _numViews;

private:
	NullRHIShaderResourceViewTable(std::shared_ptr<RHIDevice> device, ERHIShaderResourceViewType type, size_t numViews);

public:
	virtual ~NullRHIShaderResourceViewTable() noexcept override;

	virtual void CreateUnorderedAccessView(size_t viewIndex, std::shared_ptr<RHIResource> pResource, std::shared_ptr<RHIResource> pCounter, const RHIUnorderedAccessViewDesc& desc) override;
};