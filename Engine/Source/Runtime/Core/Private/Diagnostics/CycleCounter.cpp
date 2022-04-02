// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Diagnostics/CycleCounter.h"
#include "Diagnostics/CycleCounterNamespace.h"

using namespace libty;

void CycleCounter::Register(CycleCounterNamespace* Namespace)
{
	Namespaces.emplace(Namespace->GetName(), Namespace);
}

CycleCounterNamespace* CycleCounter::GetNamespace(std::wstring_view Name) const
{
	if (auto It = Namespaces.find(Name); It != Namespaces.end())
	{
		return It->second;
	}
	return nullptr;
}

CycleCounter& CycleCounter::Get()
{
	static CycleCounter Singleton;
	return Singleton;
}