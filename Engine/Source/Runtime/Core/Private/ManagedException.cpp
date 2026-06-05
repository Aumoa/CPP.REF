// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "ManagedException.h"
#include <utility>

namespace Ayla
{
	ManagedException::ManagedException(String managedTypeName, String message, String managedDetails)
		: Exception(message)
		, m_ManagedTypeName(std::move(managedTypeName))
		, m_ManagedDetails(std::move(managedDetails))
	{
	}

	ManagedException::~ManagedException() noexcept
	{
	}

	String ManagedException::ToString() const noexcept
	{
		return m_ManagedDetails.IsEmpty() ? Exception::ToString() : m_ManagedDetails;
	}
}
