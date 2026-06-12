// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"
#include "IntegralTypes.h"

namespace Ayla
{
	class CORE_API ManagedException : public Exception
	{
		String m_ManagedTypeName;
		String m_ManagedDetails;
		uint64 m_ManagedExceptionToken = 0;

	public:
		ManagedException(String managedTypeName, String message, String managedDetails, uint64 managedExceptionToken = 0);
		virtual ~ManagedException() noexcept override;

		String GetManagedTypeName() const noexcept { return m_ManagedTypeName; }
		String GetManagedDetails() const noexcept { return m_ManagedDetails; }
		uint64 GetManagedExceptionToken() const noexcept { return m_ManagedExceptionToken; }
		virtual String ToString() const noexcept override;
	};
}
