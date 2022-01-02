// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class CORE_API MemoryStatus
{
public:
	MemoryStatus(
		size_t TotalPhysicalMemory,
		size_t UsedPhysicalMemory,
		size_t ProcessUsedMemory
	);

public:
	const size_t TotalPhysicalMemory;
	const size_t UsedPhysicalMemory;
	const size_t ProcessUsedMemory;

public:
	static MemoryStatus Capture();
};