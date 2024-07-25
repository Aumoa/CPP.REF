// Copyright 2020-2023 Aumoa.lib. All right reserved.

import Core;

Name::Name(String Str)
	: Index(IndexOf(Str))
	, CachedDisplayStr(Str)
{
}

int32 Name::IndexOf(String Str)
{
	auto ScopedLock = Lock();
	auto It = IndexMap().find(Str);
	if (It == IndexMap().end())
	{
		return AllocateNameInternal(Str);
	}

	return It->second;
}

int32 Name::AllocateNameInternal(String Str)
{
	int32 Index = (int32)IndexCollection().size();
	IndexCollection().emplace_back(Str);
	IndexMap().emplace(Str, Index);
	return Index;
}

std::vector<String>& Name::IndexCollection() noexcept
{
#define DECLARE_NAME_ELEMENT(X, Index) TEXT(#X),
	static std::vector<String> Collection = []()
	{
		return std::vector<String>
		{
#include "Localizational/EmbeddedNames.inl"
		};
	}();
#undef DECLARE_NAME_ELEMENT
	return Collection;
}

std::map<String, int32, Name::NameStringComparator>& Name::IndexMap() noexcept
{
#define DECLARE_NAME_ELEMENT(X, Index) { TEXT(#X), Index },
	static std::map<String, int32, Name::NameStringComparator> Map = []()
	{
		return std::map<String, int32, Name::NameStringComparator>
		{
#include "Localizational/EmbeddedNames.inl"
		};
	}();
#undef DECLARE_NAME_ELEMENT
	return Map;
}

std::unique_lock<Spinlock> Name::Lock()
{
	static Spinlock Lck;
	return std::unique_lock(Lck);
}