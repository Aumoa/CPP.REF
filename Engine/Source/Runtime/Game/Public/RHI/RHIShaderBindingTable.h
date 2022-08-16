// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceResource.h"

class GAME_API RHIShaderBindingTable : public RHIDeviceResource
{
protected:
	RHIShaderBindingTable(std::shared_ptr<RHIDevice> device);

public:
	virtual ~RHIShaderBindingTable() noexcept override;
};