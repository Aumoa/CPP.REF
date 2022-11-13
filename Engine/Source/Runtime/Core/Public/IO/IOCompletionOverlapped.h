// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/PlatformMisc.h"
#include <vector>
#include <exception>

class CORE_API IOCompletionOverlapped
{
	uint8 _pad[PlatformMisc::IOOverlappedDefaultSize()];
	IOCompletionOverlapped* _ptr;

public:
	IOCompletionOverlapped();
	virtual ~IOCompletionOverlapped() noexcept;

	virtual bool Complete(size_t resolved) = 0;
	virtual bool Failed(std::exception_ptr ptr) = 0;

public:
	void* ToOverlapped() const noexcept;
	static IOCompletionOverlapped* FromOverlapped(void* overlapped) noexcept;
};