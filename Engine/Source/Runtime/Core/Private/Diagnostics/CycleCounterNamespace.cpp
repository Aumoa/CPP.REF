// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Diagnostics/CycleCounterNamespace.h"
#include "Diagnostics/CycleCounter.h"

CycleCounterNamespace::CycleCounterNamespace(std::wstring_view Name)
	: Name(Name)
{
}

std::wstring_view CycleCounterNamespace::GetName() const
{
	return Name;
}

void CycleCounterNamespace::Register(CycleCounterUnit* Unit)
{
	std::unique_lock Mtx_lock(Mtx);
	if (Units.size() == 0)
	{
		CycleCounter::Get().Register(this);
	}
	Units.emplace_back(Unit);
}