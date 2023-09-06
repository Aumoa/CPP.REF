// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/String.h"
#include <memory>

class CORE_API Thread
{
private:
	struct Impl_t
	{
		void* Handle = nullptr;
		size_t Refs = 0;
	};

	Impl_t* Impl = nullptr;

public:
	Thread() noexcept;
	Thread(const Thread& Thr) noexcept;
	Thread(Thread&& Thr) noexcept;
	~Thread() noexcept;

	constexpr bool IsValid() const noexcept { return Impl; }
	constexpr void* NativeHandle() const noexcept { return Impl ? Impl->Handle : nullptr; }

	void SetDescription(String InDescription) noexcept;

	Thread& operator =(const Thread& Thr) noexcept;
	Thread& operator =(Thread&& Thr) noexcept;

public:
	static Thread GetCurrentThread();
};