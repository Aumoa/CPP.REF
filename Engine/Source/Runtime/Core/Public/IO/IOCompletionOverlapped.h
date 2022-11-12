// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/PlatformMisc.h"
#include <vector>

class CORE_API IOCompletionOverlapped
{
	uint8 _pad[PlatformMisc::IOOverlappedDefaultSize()];
	IOCompletionOverlapped* _ptr;

public:
	IOCompletionOverlapped();
	virtual ~IOCompletionOverlapped() noexcept;

	virtual bool Complete() = 0;

public:
	void* ToOverlapped() const noexcept;
	static IOCompletionOverlapped* FromOverlapped(void* overlapped) noexcept;
};