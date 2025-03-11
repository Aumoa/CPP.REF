// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "System/Action.h"
#include "System/IntegralTypes.h"
#include "System/AssertionMacros.h"
#include "Platform/PlatformIO.h"

class IOCompletionOverlapped
{
	uint8 OverlappedBuffer[PlatformIO::OVERLAPPED_SIZE + sizeof(void*)];
	Action<IOCompletionOverlapped*, size_t, int32> Work;

private:
	static IOCompletionOverlapped*& SelfPtr(uint8* Memory)
	{
		return reinterpret_cast<IOCompletionOverlapped*&>(Memory[PlatformIO::OVERLAPPED_SIZE]);
	}

public:
	IOCompletionOverlapped(Action<IOCompletionOverlapped*, size_t, int32> InWork)
		: OverlappedBuffer{}
		, Work(std::move(InWork))
	{
		SelfPtr(OverlappedBuffer) = this;
	}

	~IOCompletionOverlapped() noexcept
	{
	}

	inline void Complete(size_t Resolved)
	{
		Work(this, Resolved, 0);
	}

	inline void Failed(int32 SystemCode)
	{
		Work(this, 0, SystemCode);
	}

public:
	inline void* ToOverlapped() noexcept
	{
		return OverlappedBuffer;
	}

	inline const void* ToOverlapped() const noexcept
	{
		return OverlappedBuffer;
	}

	static IOCompletionOverlapped* FromOverlapped(void* Overlapped) noexcept
	{
		uint8* Memory = reinterpret_cast<uint8*>(Overlapped);
		auto* Self = SelfPtr(Memory);
		check(Self && SelfPtr(Self->OverlappedBuffer) == Self);
		return Self;
	}
};