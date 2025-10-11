// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <stop_token>
#include "Threading/Spinlock.h"
#include "Marshal/ManagedCancellationTokenWrapper.h"
#include "TimeSpan.h"

namespace Ayla
{
	class CancellationToken;

	class CORE_API CancellationTokenSource
	{
		friend class CancellationToken;
		static ssize_t(*coreclr__create_cancellation_token_source)();
		static void(*coreclr__free_cancellation_token_source)(ssize_t);
		static void(*coreclr__cancel)(ssize_t);
		static void static__CancellationTokenSource();

	private:
		class managed_stop_source
		{
		public:
			managed_stop_source() = default;
			managed_stop_source(const managed_stop_source&) = delete;
			managed_stop_source(managed_stop_source&&) = delete;
			~managed_stop_source() noexcept;

			std::stop_source m_Source;
			Spinlock m_MarshalLock;
			ssize_t m_GCHandle;
			std::stop_callback<std::function<void()>>* m_Callback;

			void Cancel();
			void CancelManaged();
		};

	private:
		std::shared_ptr<managed_stop_source> m_Source;

	private:
		inline CancellationTokenSource(std::in_place_t)
			: m_Source{ std::make_shared<managed_stop_source>() }
		{
		}

	public:
		inline CancellationTokenSource()
		{
		}

		inline CancellationTokenSource(const CancellationTokenSource& rhs)
			: m_Source{ rhs.m_Source }
		{
		}

		inline CancellationTokenSource(CancellationTokenSource&& rhs) noexcept
			: m_Source{ std::move(rhs.m_Source) }
		{
		}

		inline ~CancellationTokenSource() noexcept
		{
		}

		CancellationToken GetToken() const;
		void Cancel();
		void CancelAfter(const TimeSpan& delay);

		inline CancellationTokenSource& operator =(const CancellationTokenSource& rhs)
		{
			m_Source = rhs.m_Source;
			return *this;
		}

		inline CancellationTokenSource& operator =(CancellationTokenSource&& rhs) noexcept
		{
			m_Source = std::move(rhs.m_Source);
			return *this;
		}

		inline bool operator ==(const CancellationTokenSource& rhs) const noexcept
		{
			return m_Source == rhs.m_Source;
		}

	public:
		static inline CancellationTokenSource Create()
		{
			return CancellationTokenSource(std::in_place);
		}

	private:
		void Invoke__Cancel();

	private:
		[[noreturn]]
		static void ThrowInvalidOperationException();
	};
}