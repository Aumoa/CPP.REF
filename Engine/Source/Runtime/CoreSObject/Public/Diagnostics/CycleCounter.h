// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <map>
#include <string>

class CycleCounterNamespace;

class CORESOBJECT_API CycleCounter
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