// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

namespace Ayla
{
	class CORE_API ManagedException : public Exception
	{
		String m_ManagedTypeName;
		String m_ManagedDetails;

	public:
		ManagedException(String managedTypeName, String message, String managedDetails);
		virtual ~ManagedException() noexcept override;

		String GetManagedTypeName() const noexcept { return m_ManagedTypeName; }
		String GetManagedDetails() const noexcept { return m_ManagedDetails; }
		virtual String ToString() const noexcept override;
	};
}
