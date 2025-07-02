// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <stop_token>
#include "Threading/CancellationToken.h"
#include "TimeSpan.h"

namespace Ayla
{
	class CORE_API CancellationTokenSource
	{
		std::shared_ptr<std::stop_source> m_Source;

	private:
		inline CancellationTokenSource(std::in_place_t)
			: m_Source{ std::make_shared<std::stop_source>() }
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
			m_Source.reset();
		}

		inline CancellationToken GetToken() const
		{
			if (m_Source == nullptr)
			{
				ThrowInvalidOperationException();
			}

			CancellationToken token;
			token.m_Source = m_Source;
			token.m_Token = m_Source->get_token();
			return token;
		}

		inline void Cancel()
		{
			if (m_Source == nullptr)
			{
				ThrowInvalidOperationException();
			}

			m_Source->request_stop();
		}

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
		[[noreturn]]
		static void ThrowInvalidOperationException();
	};
}