// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include <memory>

namespace Ayla
{
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

		static size_t GetHardwareConcurrency() noexcept;

	public:
		static Thread GetCurrentThread();
	};
}