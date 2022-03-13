// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Diagnostics/CycleCounterNamespace.h"
#include "Diagnostics/CycleCounter.h"
#include "Diagnostics/CycleCounterUnit.h"
#include "Misc/String.h"

CycleCounterNamespace::CycleCounterNamespace(std::wstring_view Name, std::wstring_view GroupName)
	: Name(Name)
	, GroupName(GroupName)
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

std::wstring CycleCounterNamespace::Trace()
{
	std::unique_lock Mtx_lock(Mtx);

	std::vector<std::wstring> Traces;
	Traces.reserve(Units.size());
	Traces.emplace_back(std::format(L"{} ({})", Name, GroupName));

	for (auto& Unit : Units)
	{
		Traces.emplace_back(std::format(L"  {:<30}: {:.4f} ms", Unit->GetName(), Unit->GetAverageTime() * 1000.0f));
	}

	return String::Join(L"\n", Traces);
}