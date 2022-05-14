// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Diagnostics/CycleCounterUnit.h"
#include "Diagnostics/CycleCounterNamespace.h"

using namespace libty;

CycleCounterUnit::CycleCounterUnit(CycleCounterNamespace* Namespace, String Name)
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

String CycleCounterUnit::GetName()
{
	return Name;
}

float CycleCounterUnit::GetAverageTime()
{
	float Sum = 0;
	size_t Total = std::min(NumStats.load(), (size_t)1024);

	for (size_t i = 0; i < Total; ++i)
	{
		Sum += Stats[i];
	}

	return Sum / (float)Total;
}