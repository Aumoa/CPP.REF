// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GC/GC.h"
#include "Object.h"

namespace Ayla
{
	void GC::Collect()
	{
		Object::s_RootCollection.Collect();
	}
}