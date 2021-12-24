// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Diagnostics/CycleCounterUnit.h"
#include "Diagnostics/CycleCounterNamespace.h"

CycleCounterUnit::CycleCounterUnit(CycleCounterNamespace* Namespace, std::wstring_view Name)
	: Namespace(Namespace)
	, Name(Name)
	, Stats({})
{
}

auto CycleCounterUnit::GetScopedTimer() -> Instance
{
	size_t Index = NumStats++;
	
	// Deferred register.
	if (Index == 0)
	{
		Namespace->Register(this);
	}

	return Instance(this, Index);
}