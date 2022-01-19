// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <mutex>

class CycleCounterUnit;

class CORESOBJECT_API CycleCounterNamespace
{
	std::wstring Name;
	std::wstring GroupName;
	std::mutex Mtx;
	std::vector<CycleCounterUnit*> Units;

public:
	CycleCounterNamespace(std::wstring_view Name, std::wstring_view GroupName);

	std::wstring_view GetName() const;

	void Register(CycleCounterUnit* Unit);
	std::wstring Trace();
};