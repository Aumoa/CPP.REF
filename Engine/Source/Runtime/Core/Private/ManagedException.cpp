// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "ManagedException.h"
#include "Marshal/ManagedExceptionInterop.h"
#include <utility>

namespace Ayla
{
	namespace
	{
		struct ManagedExceptionTokenReleaser
		{
			void operator()(uint64* token) const noexcept
			{
				if (token == nullptr)
				{
					return;
				}

				ManagedExceptionInterop::ReleaseCapturedException(*token);
				delete token;
			}
		};

		std::shared_ptr<uint64> MakeManagedExceptionToken(uint64 token)
		{
			if (token == 0)
			{
				return {};
			}

			return std::shared_ptr<uint64>(new uint64(token), ManagedExceptionTokenReleaser{});
		}
	}

	ManagedException::ManagedException(String managedTypeName, String message, String managedDetails, uint64 managedExceptionToken)
		: Exception(message)
		, m_ManagedTypeName(std::move(managedTypeName))
		, m_ManagedDetails(std::move(managedDetails))
		, m_ManagedExceptionToken(MakeManagedExceptionToken(managedExceptionToken))
	{
	}

	ManagedException::~ManagedException() noexcept
	{
	}

	uint64 ManagedException::DetachManagedExceptionToken() const noexcept
	{
		if (m_ManagedExceptionToken == nullptr)
		{
			return 0;
		}

		uint64 token = *m_ManagedExceptionToken;
		*m_ManagedExceptionToken = 0;
		return token;
	}

	String ManagedException::ToString() const noexcept
	{
		return m_ManagedDetails.IsEmpty() ? Exception::ToString() : m_ManagedDetails;
	}
}
