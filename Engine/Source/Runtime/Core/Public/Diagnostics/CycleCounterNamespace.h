// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/StringView.h"
#include "Threading/Spinlock.h"
#include <string>
#include <string_view>
#include <vector>
#include <mutex>

namespace libty::inline Core
{
	class CycleCounterUnit;

	class CORE_API CycleCounterNamespace
	{
		String Name;
		String GroupName;
		Spinlock Mtx;
		std::vector<CycleCounterUnit*> Units;

	public:
		CycleCounterNamespace(StringView Name, StringView GroupName);

		StringView GetName() const;

		void Register(CycleCounterUnit* Unit);
		String Trace();
	};
}