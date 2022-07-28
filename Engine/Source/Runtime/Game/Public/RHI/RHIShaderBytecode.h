// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIDeviceResource.h"

class GAME_API RHIShaderBytecode : public RHIDeviceResource
{
protected:
	RHIShaderBytecode(std::shared_ptr<RHIDevice> device);

public:
	virtual ~RHIShaderBytecode() noexcept override;

	virtual size_t GetBufferSize() const noexcept = 0;
	virtual const void* GetBufferPointer() const noexcept = 0;
};