// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Reflection/TypeRegister.h"
#include "Reflection/TypeCollector.h"
#include "Reflection/RuntimeType.h"

namespace Ayla
{
	void TypeRegister::Register() const
	{
		TypeCollector::AddType(std::make_unique<RuntimeType>(*this));
	}
}