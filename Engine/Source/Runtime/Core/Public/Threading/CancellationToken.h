// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "Threading/CancellationTokenSource.h"
#include <stop_token>
#include <memory>

namespace Ayla
{
	class CORE_API CancellationToken
	{
	private:
		friend class CancellationTokenSource;
		std::shared_ptr<CancellationTokenSource::managed_stop_source> m_Source;
		std::stop_token m_Token;

	public:
		inline CancellationToken() noexcept
		{
		}

		inline CancellationToken(const CancellationToken& rhs) noexcept
			: m_Source{ rhs.m_Source }
			, m_Token{ rhs.m_Token }
		{
		}

		inline CancellationToken(CancellationToken&& rhs) noexcept
			: m_Source{ std::move(rhs.m_Source) }
			, m_Token{ std::move(rhs.m_Token) }
		{
		}

		inline bool IsCancellationRequested() const noexcept
		{
			return m_Token.stop_requested();
		}

		inline bool CanBeCanceled() const noexcept
		{
			return m_Token.stop_possible();
		}

		inline void ThrowIfCancellationRequested() const
		{
			if (m_Token.stop_requested())
			{
				ThrowOperationCanceledException();
			}
		}

		ManagedCancellationTokenWrapper AsManaged();

		inline CancellationToken& operator =(const CancellationToken& rhs) noexcept
		{
			m_Source = rhs.m_Source;
			m_Token = rhs.m_Token;
			return *this;
		}

		inline CancellationToken& operator =(CancellationToken&& rhs) noexcept
		{
			m_Source = std::move(rhs.m_Source);
			m_Token = std::move(rhs.m_Token);
			return *this;
		}

		inline bool operator ==(const CancellationToken& rhs) const noexcept
		{
			return m_Source == rhs.m_Source && m_Token == rhs.m_Token;
		}

		inline explicit operator std::stop_token& () noexcept
		{
			return m_Token;
		}

		inline explicit operator const std::stop_token& () const noexcept
		{
			return m_Token;
		}

		static inline CancellationToken None()
		{
			return CancellationToken();
		}

	private:
		[[noreturn]]
		static void ThrowOperationCanceledException();
	};
}