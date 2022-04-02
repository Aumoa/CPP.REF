// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <map>
#include <string>

namespace libty::inline Core::inline Diagnostics
{
	class CycleCounterNamespace;

	class CORE_API CycleCounter
	{
		CycleCounter() = default;
		~CycleCounter() = default;

	private:
		std::map<std::wstring, CycleCounterNamespace*, std::less<>> Namespaces;

	public:
		void Register(CycleCounterNamespace* Namespace);
		CycleCounterNamespace* GetNamespace(std::wstring_view Name) const;

	public:
		static CycleCounter& Get();
	};
}