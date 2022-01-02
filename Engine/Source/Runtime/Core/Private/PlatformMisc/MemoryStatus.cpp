// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "PlatformMisc/MemoryStatus.h"

MemoryStatus::MemoryStatus(
	size_t TotalPhysicalMemory,
	size_t UsedPhysicalMemory,
	size_t ProcessUsedMemory
)
	: TotalPhysicalMemory(TotalPhysicalMemory)
	, UsedPhysicalMemory(UsedPhysicalMemory)
	, ProcessUsedMemory(ProcessUsedMemory)
{
}

#if PLATFORM_WINDOWS

#include <Windows.h>
#include <Psapi.h>

MemoryStatus MemoryStatus::Capture()
{
	MEMORYSTATUSEX StatusEx;
	StatusEx.dwLength = sizeof(MEMORYSTATUSEX);
	BOOL bStatus = GlobalMemoryStatusEx(&StatusEx);
	check(bStatus);

	PROCESS_MEMORY_COUNTERS ProcessMem;
	bStatus = GetProcessMemoryInfo(GetCurrentProcess(), &ProcessMem, sizeof(ProcessMem));
	check(bStatus);

	return MemoryStatus(
		(size_t)StatusEx.ullTotalPhys,
		(size_t)(StatusEx.ullTotalPhys - StatusEx.ullAvailPhys),
		(size_t)ProcessMem.WorkingSetSize
	);
}

#endif