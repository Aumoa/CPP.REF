// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Diagnostics/CycleCounterNamespace.h"
#include "Diagnostics/CycleCounter.h"
#include "Diagnostics/CycleCounterUnit.h"
#include "Misc/String.h"

using namespace libty;

CycleCounterNamespace::CycleCounterNamespace(String Name, String GroupName)
	: Name(Name)
	, GroupName(GroupName)
{
}

String CycleCounterNamespace::GetName() const
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

String CycleCounterNamespace::Trace()
{
	std::unique_lock Mtx_lock(Mtx);

	std::vector<String> Traces;
	Traces.reserve(Units.size());
	Traces.emplace_back(String::Format(TEXT("{} ({})"), Name, GroupName));

	for (auto& Unit : Units)
	{
		Traces.emplace_back(String::Format(TEXT("  {:<30}: {:.4f} ms"), Unit->GetName(), Unit->GetAverageTime() * 1000.0f));
	}

	return String::Join(TEXT("\n"), Traces);
}