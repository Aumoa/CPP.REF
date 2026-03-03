// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Action.h"
#include "NonCopyable.h"
#include "IntegralTypes.h"
#include "AssertionMacros.h"
#include "Platform/PlatformIO.h"
#include <functional>

namespace Ayla
{
	class CORE_API IOCompletionOverlapped : public NonCopyable
	{
		static size_t s_Refs;

		uint8 OverlappedBuffer[PlatformIO::OVERLAPPED_SIZE + sizeof(void*)];
		std::move_only_function<void(size_t, int32)> m_CompletionWork;

	private:
		static IOCompletionOverlapped*& SelfPtr(uint8* Memory)
		{
			return reinterpret_cast<IOCompletionOverlapped*&>(Memory[PlatformIO::OVERLAPPED_SIZE]);
		}

	public:
		IOCompletionOverlapped();
		~IOCompletionOverlapped() noexcept;

		inline void SetOnCompletion(std::move_only_function<void(size_t, int32)> callback)
		{
			m_CompletionWork = std::move(callback);
		}

		inline void Complete(size_t Resolved)
		{
			std::exchange(m_CompletionWork, {})(Resolved, 0);
		}

		inline void Failed(int32 SystemCode)
		{
			std::exchange(m_CompletionWork, {})(0, SystemCode);
		}

		static void Assert();

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
}