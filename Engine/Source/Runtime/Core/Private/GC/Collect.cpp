// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GC/Collect.h"
#include "Object.h"

void Ayla::GC::Collect()
{
	Object::s_RootCollection.Collect();
}