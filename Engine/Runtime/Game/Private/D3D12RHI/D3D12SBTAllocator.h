// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "D3D12Minimal.h"

class D3D12SBTAllocator : virtual public Object
{
public:
	using Super = Object;

public:
	D3D12SBTAllocator();
	~D3D12SBTAllocator() override;

	void ReallocAndStart(size_t num);
};