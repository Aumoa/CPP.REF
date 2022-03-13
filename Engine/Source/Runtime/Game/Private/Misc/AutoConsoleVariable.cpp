// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/AutoConsoleVariable.h"

DEFINE_LOG_CATEGORY(LogConsoleVar);

namespace Details
{
	static std::map<std::wstring, AutoConsoleVariableBase*>* StaticVars;

	AutoConsoleVariableBase::AutoConsoleVariableBase(std::wstring_view Key)
	{
		static std::map<std::wstring, AutoConsoleVariableBase*> StaticVars_Impl;
		StaticVars = &StaticVars_Impl;

		Name = std::wstring(Key);
		std::wstring Key_v = String::ToLower(Name);

		auto [It, bResult] = StaticVars_Impl.emplace(Key_v, this);
		checkf(bResult, L"Duplicated AutoConsoleVariable detected. Key: '{}'", Key);
	}

	std::wstring_view AutoConsoleVariableBase::GetName() const
	{
		return Name;
	}

	bool AutoConsoleVariableBase::TryProcessConsoleVar(std::wstring_view Key, std::wstring_view Arguments)
	{
		std::wstring Key_v = String::ToLower(std::wstring(Key));

		auto It = StaticVars->find(Key_v);
		if (It == StaticVars->end())
		{
			return false;
		}

		It->second->ProcessConsoleVar(Arguments);
		return true;
	}
}