// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"

namespace libty::inline Core
{
	struct CORESOBJECT_API IDisposable : implements(SObject)
	{
		GENERATED_BODY(IDisposable);

	public:
		virtual void Dispose() = 0;
		virtual bool IsDisposed() = 0;

	protected:
		// Special Interface Constructor.
		IDisposable();
	};
}