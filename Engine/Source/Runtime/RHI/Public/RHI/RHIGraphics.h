// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NRHICommandQueue;

class RHI_API NRHIGraphics
{
protected:
	NRHIGraphics();

public:
	virtual ~NRHIGraphics() noexcept;

	virtual void Init() = 0;
	virtual std::shared_ptr<NRHICommandQueue> CreateCommandQueue() = 0;

public:
	static std::unique_ptr<NRHIGraphics> GenerateGraphics(String InRHIModuleName);
};