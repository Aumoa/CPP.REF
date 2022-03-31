// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "ScopedTimer.h"
#include <array>
#include <string>
#include <atomic>

class CycleCounterNamespace;

class CORE_API CycleCounterUnit
{
	CycleCounterNamespace* Namespace;
	std::wstring Name;
	std::array<float, 1024> Stats;
	std::atomic<size_t> NumStats = 0;

public:
	class Instance
	{
		friend class CycleCounterUnit;

		CycleCounterUnit* Source;
		size_t Index;
		ScopedTimer<Instance> Timer;

		Instance(CycleCounterUnit* Source, size_t Index)
			: Source(Source)
			, Index(Index % Source->Stats.size())
			, Timer(this, &Instance::Report)
		{
		}

	public:
		~Instance() = default;

	private:
		void Report(float Time)
		{
			Source->Stats[Index] = Time;
		}
	};

public:
	CycleCounterUnit(CycleCounterNamespace* NamespacePtr, std::wstring_view Name);

	Instance GetScopedTimer();
	std::wstring GetName();
	float GetAverageTime();
};