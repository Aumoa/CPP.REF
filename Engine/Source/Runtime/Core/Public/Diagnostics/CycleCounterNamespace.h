// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <mutex>

class CycleCounterUnit;

class CORE_API CycleCounterNamespace
{
	std::wstring Name;
	std::mutex Mtx;
	std::vector<CycleCounterUnit*> Units;

public:
	CycleCounterNamespace(std::wstring_view Name);

	std::wstring_view GetName() const;

	void Register(CycleCounterUnit* Unit);
};