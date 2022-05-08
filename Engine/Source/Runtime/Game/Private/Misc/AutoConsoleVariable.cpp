// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/AutoConsoleVariable.h"

DEFINE_LOG_CATEGORY(LogConsoleVar);

namespace libty::inline Game::Details
{
	static std::map<String, AutoConsoleVariableBase*>* StaticVars;

	AutoConsoleVariableBase::AutoConsoleVariableBase(StringView Key)
	{
		static std::map<String, AutoConsoleVariableBase*> StaticVars_Impl;
		StaticVars = &StaticVars_Impl;

		Name = String(Key);
		String Key_v = String::ToLower(Name);

		auto [It, bResult] = StaticVars_Impl.emplace(Key_v, this);
		checkf(bResult, TEXT("Duplicated AutoConsoleVariable detected. Key: '{}'"), Key);
	}

	StringView AutoConsoleVariableBase::GetName() const
	{
		return Name;
	}

	bool AutoConsoleVariableBase::TryProcessConsoleVar(StringView Key, StringView Arguments)
	{
		String Key_v = String::ToLower(Key);

		auto It = StaticVars->find(Key_v);
		if (It == StaticVars->end())
		{
			return false;
		}

		It->second->ProcessConsoleVar(Arguments);
		return true;
	}
}