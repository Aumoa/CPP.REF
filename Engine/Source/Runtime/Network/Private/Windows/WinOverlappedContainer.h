// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#include "CoreMinimal.h"
#include "WinOverlapped.h"

struct WinOverlappedContainer
{
	std::atomic<size_t> Ref;
	WinOverlapped Overlaps[(size_t)EIOCPAsyncOp::Count] = {};

	WinOverlappedContainer() noexcept;
	~WinOverlappedContainer() noexcept;

	void AddRef() noexcept;
	void Release() noexcept;

	WinOverlapped* GetRef(EIOCPAsyncOp op) noexcept;
};

#endif