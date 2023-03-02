// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class RHI_API NRHIGraphics
{
protected:
	NRHIGraphics();

public:
	virtual ~NRHIGraphics() noexcept;

	virtual void Init() = 0;

public:
	static std::unique_ptr<NRHIGraphics> GenerateGraphics(String InRHIModuleName);
};