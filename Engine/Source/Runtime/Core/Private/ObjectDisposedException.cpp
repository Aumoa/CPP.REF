// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "ObjectDisposedException.h"

namespace Ayla
{
	ObjectDisposedException::ObjectDisposedException(String objectName)
		: Exception(String::Format(TEXT("{0} is already disposed."), objectName))
	{
	}

	void ObjectDisposedException::ThrowIfDisposed(bool disposed, String objectName)
	{
		if (disposed)
		{
			throw ObjectDisposedException(objectName);
		}
	}
}