// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/AutoConsoleVariable.h"

DEFINE_LOG_CATEGORY(LogConsoleVar);

namespace libty::inline Game::Details
{
	static std::map<String, AutoConsoleVariableBase*>* StaticVars;

	AutoConsoleVariableBase::AutoConsoleVariableBase(String Key)
	{
		static std::map<String, AutoConsoleVariableBase*> StaticVars_Impl;
		StaticVars = &StaticVars_Impl;

		Name = String(Key);
		String Key_v = Name.ToLower();

		auto [It, bResult] = StaticVars_Impl.emplace(Key_v, this);
		checkf(bResult, TEXT("Duplicated AutoConsoleVariable detected. Key: '{}'"), Key);
	}

	String AutoConsoleVariableBase::GetName() const
	{
		return Name;
	}

	bool AutoConsoleVariableBase::TryProcessConsoleVar(String Key, String Arguments)
	{
		String Key_v = Key.ToLower();

		auto It = StaticVars->find(Key_v);
		if (It == StaticVars->end())
		{
			return false;
		}

		It->second->ProcessConsoleVar(Arguments);
		return true;
	}
}