// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceResource.h"

class RHIResource;
struct RHIUnorderedAccessViewDesc;

class GAME_API RHIShaderResourceViewTable : public RHIDeviceResource
{
protected:
	RHIShaderResourceViewTable(std::shared_ptr<RHIDevice> device);

public:
	virtual ~RHIShaderResourceViewTable() noexcept override;

	virtual void CreateUnorderedAccessView(size_t viewIndex, std::shared_ptr<RHIResource> pResource, std::shared_ptr<RHIResource> pCounter, const RHIUnorderedAccessViewDesc& desc) = 0;
};