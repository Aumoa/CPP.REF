// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"
#include "Platform/PlatformMacros.h"

namespace Ayla
{
	class CORE_API ObjectDisposedException : public Exception
	{
	public:
		ObjectDisposedException(String objectName)
			: Exception(String::Format(TEXT("{0} is already disposed."), objectName))
		{
		}

		static void ThrowIfDisposed(bool disposed, String objectName)
		{
			if (disposed)
			{
				throw ObjectDisposedException(objectName);
			}
		}
	};
}