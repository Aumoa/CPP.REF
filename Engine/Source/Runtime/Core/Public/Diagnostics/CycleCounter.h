// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/String.h"
#include <map>
#include <string>

namespace libty::inline Core
{
	class CycleCounterNamespace;

	class CORE_API CycleCounter
	{
		CycleCounter() = default;
		~CycleCounter() = default;

	private:
		std::map<String, CycleCounterNamespace*, std::less<>> Namespaces;

	public:
		void Register(CycleCounterNamespace* Namespace);
		CycleCounterNamespace* GetNamespace(String Name) const;

	public:
		static CycleCounter& Get();
	};
}